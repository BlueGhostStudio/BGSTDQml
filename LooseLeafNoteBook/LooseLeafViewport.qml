import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BGStudio.Controls
import BGStudio.Handwriter

HWViewport {
    id: viewport

    enum UiMode {
        WritingMode,
        NavigationMode
    }

    property int uiMode: LooseLeafViewport.WritingMode
    property Action actFlipBack: Action {
        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/return.png"

        onTriggered: flipBack()
    }
    property Action actSave: Action {
        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/save.png"

        enabled: viewport.paper && viewport.paper.isModified

        onTriggered: {
            if (paper)
                viewport.paper.save()
        }
    }
    property Action actClose: Action {
        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/close.png"

        onTriggered: {
            viewport.paper.close()
        }
    }
    property Action actToggleToolbar: Action {
        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/toggle_toolbar.png"

        onTriggered: {
            uiMode = uiMode === LooseLeafViewport.WritingMode
                    ? LooseLeafViewport.NavigationMode
                    : LooseLeafViewport.WritingMode
        }
    }

    signal flipBack()

    maxZoomRatio: 10
    zoomFactor: 1

    clip: true

    HWWriteNavigator {
        navButtonOrientation: viewport.width > viewport.height ? Qt.Vertical : Qt.Horizontal

        extToolBar0Actions: [
            actFlipBack,
            actToggleToolbar
            /*Action {
                icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/toggle_toolbar.png"

                onTriggered: {
                    uiMode = uiMode === LooseLeafViewport.WritingMode
                            ? LooseLeafViewport.NavigationMode
                            : LooseLeafViewport.WritingMode
                }
            }*/
        ]
    }

    /*Connections {
        target: ActivePaperList
        onClosingPaper: (pid) => {
                            if (paper && paper.localStorageID === pid) {
                                paper = null
                            }
                        }
    }*/

    function assignPaper(paper) {
        viewport.paper = paper
    }

    function unassignPaper(pid) {
        if (paper && paper.localStorageID === pid) {
            viewport.paper = null
            return true
        } else
            return false
    }
}
