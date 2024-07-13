pragma Singleton

import QtQuick
import BGStudio.Handwriter

QtObject {
    property ListModel activePapersIndex: ListModel {}
    property list<LooseLeafPaper> activePapers: []
    property Component cmpPaper: Component {
        LooseLeafPaper {}
    }

    signal paperOpened(var paper)
    signal paperClosing(int pid)
    // signal paperClosed(int pid)

    function openPaper(pid) {
        if (pid > 0) {
            var paper = activePapers.find(
                        (p) => {
                            return p.localStorageID === pid
                        })

            if (!paper) {
                paper = cmpPaper.createObject(null)
                paper.loadLocalStorageData(pid)
                activePapers.push(paper)
                activePapersIndex.append({ "pid": pid })
            }
        }

        paperOpened(paper)

        return paper
    }
    function closePaper(pid) {
        if (pid > 0) {
            let i = activePapers.findIndex((p) => p.localStorageID === pid)
            if (i !== -1) {
                paperClosing(pid)

                let paper = activePapers.splice(i, 1)[0]
                paper.destroy()
            }

            for (i = 0; i < activePapersIndex.count; i++) {
                if (activePapersIndex.get(i).pid === pid) {
                    activePapersIndex.remove(i)
                    break
                }
            }
        }

        /*const i = findActivePaperIndex(pid)
        if (i !== -1)
            activePapersIndex.remove(i)*/
    }
}
