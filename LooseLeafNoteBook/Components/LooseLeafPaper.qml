import QtQuick 2.15

import BGStudio.Handwriter

QtObject {
    property int localStorageID

    property HWPaper writerPaper: HWPaper {
        property bool loaded: false
    }
    property HWPaperPreview previewPaper: HWPaperPreview {}
    property Item activedPaper: writerPaper

    function loadLocalStorageData(pid) {
        if (localStorageID !== pid) {
            activedPaper.loadLocalStorageData(pid)
            localStorageID = pid
            writerPaper.loaded = activedPaper === writerPaper
        }
    }

    function switchWritePaper () {
        if (activedPaper !== writerPaper) {
            previewPaper.parent = null
            activedPaper = writerPaper

            if (!writerPaper.loaded) {
                writerPaper.loadLocalStorageData(localStorageID)
                writerPaper.loaded = true
            }
        }
    }

    function switchPreviewPaper () {
        if (activedPaper !== previewPaper) {
            writerPaper.parent = null
            activedPaper = previewPaper
        }
    }
}
