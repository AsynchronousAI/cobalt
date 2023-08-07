//import * as roblox from "./roblox"
import * as cobalt from "./cobalt"
import * as timers from "timers"
import * as util from "./util"
import * as vscode from "vscode"
import { Diagnostic, Severity, Label } from "./structures/diagnostic"
import { Output } from "./structures/output"
import { lintConfig } from "./configLint"
import { byteToCharMap } from "./byteToCharMap"
import { Capabilities } from "./structures/capabilities"

let trycobalt: Promise<boolean>

enum RunType {
    OnSave = "onSave",
    OnType = "onType",
    OnNewLine = "onNewLine",
    OnIdle = "onIdle",
}

function labelToRange(
    document: vscode.TextDocument,
    label: Label,
    byteOffsetMap: Map<number, number>,
): vscode.Range {
    return new vscode.Range(
        document.positionAt(
            byteOffsetMap.get(label.span.start) ?? label.span.start,
        ),
        document.positionAt(
            byteOffsetMap.get(label.span.end) ?? label.span.end,
        ),
    )
}

export async function activate(
    context: vscode.ExtensionContext,
): Promise<void> {
    console.log("cobalt-vscode activated")

    let capabilities: Capabilities = {}

    trycobalt = util
        .ensurecobaltExists(context.globalStorageUri)
        .then(() => {
            cobalt
                .cobaltCommand(
                    context.globalStorageUri,
                    "capabilities --display-style=json2",
                    cobalt.Expectation.Stdout,
                )
                .then((output) => {
                    if (output === null) {
                        return
                    }

                    capabilities = JSON.parse(output.toString())
                })
                .catch(() => {
                    // cobalt version is too old
                    return
                })
        })
        .then(() => {
            return true
        })
        .catch((error) => {
            vscode.window.showErrorMessage(
                `An error occurred when finding cobalt:\n${error}`,
            )
            return false
        })

    await trycobalt

    context.subscriptions.push(
        vscode.commands.registerCommand("cobalt.reinstall", () => {
            trycobalt = util
                .downloadcobalt(context.globalStorageUri)
                .then(() => true)
                .catch(() => false)
            return trycobalt
        }),
    )

    const diagnosticsCollection =
        vscode.languages.createDiagnosticCollection("cobalt")
    context.subscriptions.push(diagnosticsCollection)

    //let hasWarnedAboutRoblox = false

    async function lint(document: vscode.TextDocument) {
        if (!(await trycobalt)) {
            return
        }

        switch (document.languageId) {
            case "cobalt":
                break
            case "toml":
            case "yaml":
                await lintConfig(
                    capabilities,
                    context,
                    document,
                    diagnosticsCollection,
                )
                return
            default:
                return
        }

        const output = await cobalt.cobaltCommand(
            context.globalStorageUri,
            "--display-style=json2 --no-summary -",
            cobalt.Expectation.Stderr,
            vscode.workspace.getWorkspaceFolder(document.uri),
            document.getText(),
        )

        if (!output) {
            diagnosticsCollection.delete(document.uri)
            return
        }

        const diagnostics: vscode.Diagnostic[] = []
        const dataToAdd: Diagnostic[] = []
        const byteOffsets = new Set<number>()

        for (const line of output.split("\n")) {
            if (!line) {
                continue
            }

            let output: Output

            try {
                output = JSON.parse(line)
            } catch {
                console.error(`Couldn't parse output: ${line}`)
                continue
            }

            switch (output.type) {
                case "Diagnostic":
                    dataToAdd.push(output)
                    byteOffsets.add(output.primary_label.span.start)
                    byteOffsets.add(output.primary_label.span.end)
                    for (const label of output.secondary_labels) {
                        byteOffsets.add(label.span.start)
                        byteOffsets.add(label.span.end)
                    }
                    break
                case "InvalidConfig":
                    break
            }
        }

        const byteOffsetMap = byteToCharMap(document, byteOffsets)

        for (const data of dataToAdd) {
            let message = data.message
            if (data.primary_label.message.length > 0) {
                message += `\n${data.primary_label.message}`
            }

            if (data.notes.length > 0) {
                message += `\n${data.notes.map((note) => `note: ${note}\n`)}`
            }

            const diagnostic = new vscode.Diagnostic(
                labelToRange(document, data.primary_label, byteOffsetMap),
                message,
                data.severity === Severity.Error
                    ? vscode.DiagnosticSeverity.Error
                    : vscode.DiagnosticSeverity.Warning,
            )

            diagnostic.source = `cobalt::${data.code}`

            if (data.code === "unused_variable") {
                diagnostic.tags = [vscode.DiagnosticTag.Unnecessary]
            }

            diagnostic.relatedInformation = data.secondary_labels.map(
                (label) => {
                    return {
                        message: label.message,
                        location: {
                            uri: document.uri,
                            range: labelToRange(document, label, byteOffsetMap),
                        },
                    }
                },
            )

            //if (
            //    vscode.workspace
            //        .getConfiguration("cobalt")
            //        .get<boolean>("warnRoblox")
            //) {
                //if (
                  //  !hasWarnedAboutRoblox &&
                    //roblox.processDiagnostic(data, document)
                //) {
                  //  hasWarnedAboutRoblox = true
                //}
            //}

            diagnostics.push(diagnostic)
        }

        diagnosticsCollection.set(document.uri, diagnostics)
    }

    let lastTimeout: NodeJS.Timeout
    function listenToChange() {
        switch (
            vscode.workspace.getConfiguration("cobalt").get<RunType>("run")
        ) {
            case RunType.OnSave:
                return vscode.workspace.onDidSaveTextDocument(lint)
            case RunType.OnType:
                return vscode.workspace.onDidChangeTextDocument((event) =>
                    lint(event.document),
                )
            case RunType.OnNewLine:
                return vscode.workspace.onDidChangeTextDocument((event) => {
                    // Contrary to removing lines, adding new lines will leave the range at the same value hence the string comparisons
                    if (
                        event.contentChanges.some(
                            (content) =>
                                !content.range.isSingleLine ||
                                content.text === "\n" ||
                                content.text === "\r\n",
                        )
                    ) {
                        lint(event.document)
                    }
                })
            case RunType.OnIdle: {
                const idleDelay = vscode.workspace
                    .getConfiguration("cobalt")
                    .get<number>("idleDelay") as number

                return vscode.workspace.onDidChangeTextDocument((event) => {
                    timers.clearTimeout(lastTimeout)
                    lastTimeout = timers.setTimeout(
                        lint,
                        idleDelay,
                        event.document,
                    )
                })
            }
        }
    }

    let disposable = listenToChange()
    vscode.workspace.onDidChangeConfiguration((event) => {
        if (
            event.affectsConfiguration("cobalt.run") ||
            event.affectsConfiguration("cobalt.idleDelay")
        ) {
            disposable?.dispose()
            disposable = listenToChange()
        }
    })

    vscode.workspace.onDidOpenTextDocument(lint)
    vscode.workspace.onWillDeleteFiles((event) => {
        for (const documentUri of event.files) {
            diagnosticsCollection.set(documentUri, [])
        }
    })
    vscode.window.onDidChangeActiveTextEditor((editor) => {
        if (editor !== undefined) {
            lint(editor.document)
        }
    })
}

// this method is called when your extension is deactivated
export function deactivate(): void {
    return
}
