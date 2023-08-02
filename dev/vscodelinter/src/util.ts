import * as os from "os"
import * as luax from "./luax"
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

let getLatestluaxReleasePromise: Promise<GithubRelease>

export async function getLatestluaxRelease(): Promise<GithubRelease> {
    if (getLatestluaxReleasePromise) {
        return getLatestluaxReleasePromise
    }

    return request(GITHUB_RELEASES, {
        headers: {
            "User-Agent": "luax-vscode",
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

function getluaxFilename(): string {
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

function getluaxFilenamePattern(): RegExp {
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

export async function downloadluax(directory: vscode.Uri): Promise<void> {
    vscode.window.showInformationMessage("Downloading Luax Extension...")

    const filename = getluaxFilename()
    const filenamePattern = getluaxFilenamePattern()
    const release = await getLatestluaxRelease().catch((error) => {
        vscode.window.showErrorMessage(
            `Couldn't look for new luax release to download.\n\n${error.toString()}`,
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
                        "User-Agent": "luax-vscode",
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

export async function getluaxPath(
    storagePath: vscode.Uri,
): Promise<vscode.Uri | undefined> {
    const settingPath = vscode.workspace
        .getConfiguration("luax")
        .get<string | null>("luaxPath")
    if (settingPath) {
        return vscode.Uri.file(settingPath)
    }

    const downloadPath = vscode.Uri.joinPath(storagePath, getluaxFilename())
    if (await fileExists(downloadPath)) {
        return downloadPath
    }
}

export async function ensureluaxExists(
    storagePath: vscode.Uri,
): Promise<void> {
    const path = await getluaxPath(storagePath)

    if (path === undefined) {
        await vscode.workspace.fs.createDirectory(storagePath)
        return downloadluax(storagePath)
    } else {
        if (!(await fileExists(path))) {
            return Promise.reject("Path given for luax does not exist")
        }

        const version = (
            await luax.luaxCommand(
                storagePath,
                "--version",
                luax.Expectation.Stdout,
            )
        )?.trim()

        return getLatestluaxRelease()
            .then((release) => {
                if (version !== `luax ${release.tag_name}`) {
                    openUpdatePrompt(storagePath, release)
                }
            })
            .catch((error) => {
                vscode.window.showErrorMessage(
                    `Couldn't look for new luax releases.\n\n${error.toString()}`,
                )
            })
    }
}

function openUpdatePrompt(directory: vscode.Uri, release: GithubRelease) {
    vscode.window
        .showInformationMessage(
            `There's an update available for luax: ${release.tag_name}`,
            "Install Update",
            "Later",
            "Release Notes",
        )
        .then((option) => {
            switch (option) {
                case "Install Update":
                    downloadluax(directory).then(() =>
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
