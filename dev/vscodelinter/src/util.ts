import * as os from "os"
import * as lxx from "./lxx"
import * as requestNative from "request"
import * as request from "request-promise-native"
import * as unzip from "unzipper"
import * as vscode from "vscode"

import fsWriteFileAtomic = require("fs-write-stream-atomic")

const GITHUB_RELEASES = "https://summer-bonus-a893.boyned.workers.dev"

interface GithubRelease {
    assets: {
        name: string
        browser_download_url: string
    }[]
    html_url: string
    tag_name: string
}

let getLatestlxxReleasePromise: Promise<GithubRelease>

export async function getLatestlxxRelease(): Promise<GithubRelease> {
    if (getLatestlxxReleasePromise) {
        return getLatestlxxReleasePromise
    }

    return request(GITHUB_RELEASES, {
        headers: {
            "User-Agent": "lxx-vscode",
        },
    }).then((body) => {
        return JSON.parse(body) as GithubRelease
    })
}

export function platformIsSupported(): boolean {
    switch (os.platform()) {
        case "darwin":
        case "linux":
        case "win32":
            return true
        default:
            return false
    }
}

function getlxxFilename(): string {
    switch (os.platform()) {
        case "win32":
            return "selene.exe"
        case "linux":
        case "darwin":
            return "selene"
        default:
            throw new Error("Platform not supported")
    }
}

function getlxxFilenamePattern(): RegExp {
    switch (os.platform()) {
        case "win32":
            return /selene-[^-]+-windows.zip/
        case "linux":
            return /selene-[^-]+-linux.zip/
        case "darwin":
            return /selene-[^-]+-macos.zip/
        default:
            throw new Error("Platform not supported")
    }
}

async function fileExists(filename: vscode.Uri): Promise<boolean> {
    try {
        await vscode.workspace.fs.stat(filename)
        return true
    } catch (error) {
        if (error instanceof vscode.FileSystemError) {
            return error.code !== "FileNotFound"
        } else {
            throw error
        }
    }
}

export async function downloadlxx(directory: vscode.Uri): Promise<void> {
    vscode.window.showInformationMessage("Downloading lxx Extension...")

    const filename = getlxxFilename()
    const filenamePattern = getlxxFilenamePattern()
    const release = await getLatestlxxRelease().catch((error) => {
        vscode.window.showErrorMessage(
            `Couldn't look for new lxx release to download.\n\n${error.toString()}`,
        )

        return Promise.reject(error)
    })

    for (const asset of release.assets) {
        if (filenamePattern.test(asset.name)) {
            const file = fsWriteFileAtomic(
                vscode.Uri.joinPath(directory, filename).fsPath,
                {
                    mode: 0o755,
                },
            )

            return new Promise((resolve, reject) => {
                requestNative(asset.browser_download_url, {
                    headers: {
                        "User-Agent": "lxx-vscode",
                    },
                })
                    .pipe(unzip.Parse())
                    .on("entry", (entry: unzip.Entry) => {
                        if (entry.path !== filename) {
                            entry.autodrain()
                            return
                        }

                        entry
                            .pipe(file)
                            .on("finish", resolve)
                            .on("error", reject)
                    })
            })
        }
    }
}

export async function getlxxPath(
    storagePath: vscode.Uri,
): Promise<vscode.Uri | undefined> {
    const settingPath = vscode.workspace
        .getConfiguration("lxx")
        .get<string | null>("lxxPath")
    if (settingPath) {
        return vscode.Uri.file(settingPath)
    }

    const downloadPath = vscode.Uri.joinPath(storagePath, getlxxFilename())
    if (await fileExists(downloadPath)) {
        return downloadPath
    }
}

export async function ensurelxxExists(
    storagePath: vscode.Uri,
): Promise<void> {
    const path = await getlxxPath(storagePath)

    if (path === undefined) {
        await vscode.workspace.fs.createDirectory(storagePath)
        return downloadlxx(storagePath)
    } else {
        if (!(await fileExists(path))) {
            return Promise.reject("Path given for lxx does not exist")
        }

        const version = (
            await lxx.lxxCommand(
                storagePath,
                "--version",
                lxx.Expectation.Stdout,
            )
        )?.trim()

        return getLatestlxxRelease()
            .then((release) => {
                if (version !== `lxx ${release.tag_name}`) {
                    openUpdatePrompt(storagePath, release)
                }
            })
            .catch((error) => {
                vscode.window.showErrorMessage(
                    `Couldn't look for new lxx releases.\n\n${error.toString()}`,
                )
            })
    }
}

function openUpdatePrompt(directory: vscode.Uri, release: GithubRelease) {
    vscode.window
        .showInformationMessage(
            `There's an update available for lxx: ${release.tag_name}`,
            "Install Update",
            "Later",
            "Release Notes",
        )
        .then((option) => {
            switch (option) {
                case "Install Update":
                    downloadlxx(directory).then(() =>
                        vscode.window.showInformationMessage(
                            "Update succeeded.",
                        ),
                    )
                    break
                case "Release Notes":
                    vscode.env.openExternal(vscode.Uri.parse(release.html_url))
                    openUpdatePrompt(directory, release)
                    break
            }
        })
}
