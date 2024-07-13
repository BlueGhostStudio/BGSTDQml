import QtQuick
import BGStudio.Handwriter

HWPaper {
    id: hwPaper
    property bool isModified: false

    canvas: HWWriter {
        id: writer

        property alias isModified: hwPaper.isModified

        onWriteUpdated: () => {
                            isModified = true
                        }
    }

    PaperPanBehavior on x {}
    PaperPanBehavior on y {}

    function save() {
        saveLocalStorageData()
        isModified = false
    }
    function saveNClose() {
        if (isModified)
            save()
        ActivePaperList.closePaper(localStorageID)
    }

    function close() {
        ActivePaperList.closePaper(localStorageID)
    }
}
