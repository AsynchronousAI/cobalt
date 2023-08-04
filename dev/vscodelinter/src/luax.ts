import * as childProcess from "child_process"
import * as vscode from "vscode"
import * as util from "./util"

export enum Expectation {
    Stderr,
    Stdout,
}

export async function lxxCommand(
    storagePath: vscode.Uri,
    command: string,
    expectation: Expectation,
    workspace?: vscode.WorkspaceFolder,
    stdin?: string,
): Promise<string | null> {
    return util.getlxxPath(storagePath).then((lxxPath) => {
        return new Promise((resolve, reject) => {
            if (lxxPath === undefined) {
                return reject("Could not find lxx.")
            }

            const workspaceFolders = vscode.workspace.workspaceFolders

            const child = childProcess.exec(
                `"${lxxPath.fsPath}" ${command}`,
                {
                    cwd:
                        workspace?.uri.fsPath ||
                        (workspaceFolders && workspaceFolders[0])?.uri?.fsPath,
                },
                (error, stdout) => {
                    if (expectation === Expectation.Stderr) {
                        resolve(error && stdout)
                    } else {
                        if (error) {
                            reject(error)
                        } else {
                            resolve(stdout)
                        }
                    }
                },
            )

            if (stdin !== undefined) {
                child.stdin?.write(stdin)
                child.stdin?.end()
            }
        })
    })
}
