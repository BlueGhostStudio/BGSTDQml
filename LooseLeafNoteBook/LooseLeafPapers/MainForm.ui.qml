import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BGStudio.Controls

Page {
    id: page

    property ActivePaperList activePaperList: ActivePaperList {
        cmpPaper: LooseLeafPaper {}
    }

    ToolBar {
        id: toolBar

        width: parent.width

        visible: page.state === "looseLeafPaperListShow"
                 || looseLeafViewport.uiMode === LooseLeafViewport.NavigationMode

        RowLayout {
            id: listTools

            spacing: 0

            Text {
                text: "Looseleaf Papers"
                BGText.textScale: BGText.UiHeader
            }
        }
        RowLayout {
            id: viewportTools

            spacing: 0

            ToolButton {
                action: looseLeafViewport.actFlipBack
            }

            ToolButton {
                action: looseLeafViewport.actSave
            }

            Text {
                Layout.fillWidth: true
                text: "Writing"
                BGText.textScale: BGText.UiHeader
            }

            ToolButton {
                action: looseLeafViewport.actMenu
            }

            ToolButton {
                action: looseLeafViewport.actClose
            }
        }
    }

    ActivePaperTabBar {
        id: activePaperTabBar

        visible: page.state === "looseLeafPaperListShow"
                 || looseLeafViewport.uiMode === LooseLeafViewport.NavigationMode

        viewport: looseLeafViewport

        width: parent.width
        y: parent.height - height
    }

    state: "looseLeafPaperListShow"

    Flipable {
        id: flipable

        anchors.fill: parent

        front: LooseLeafPaperList {
            id: looseLeafPaperList

            anchors.fill: parent
            anchors.topMargin: toolBar.height
            anchors.bottomMargin: activePaperTabBar.height

            contentItem.anchors.margins: 10

            viewport: looseLeafViewport
        }

        back: LooseLeafViewport {
            id: looseLeafViewport
            anchors.fill: parent
            anchors.topMargin: uiMode === LooseLeafViewport.WritingMode ? 0 : toolBar.height
            anchors.bottomMargin: uiMode
                                  === LooseLeafViewport.WritingMode ? 0 : activePaperTabBar.height

            Connections {
                target: looseLeafViewport
                function onFlipBack() {
                    state = "looseLeafPaperListShow"
                }
            }
        }

        Connections {
            target: activePaperList

            function onPaperOpened(paper) {
                looseLeafViewport.assignPaper(paper)

                if (state == "looseLeafPaperListShow") {
                    state = "writing"
                    looseLeafViewport.uiMode = LooseLeafViewport.WritingMode
                }
            }
            function onPaperClosing(pid) {
                if (looseLeafViewport.unassignPaper(pid))
                    state = "looseLeafPaperListShow"
            }
        }

        transform: Rotation {
            id: rotation
            origin.x: flipable.width / 2
            origin.y: flipable.height / 2
            axis.x: 0
            axis.y: 1
            axis.z: 0
            angle: 0
        }
    }

    states: [
        State {
            name: "looseLeafPaperListShow"

            PropertyChanges {
                target: listTools
                visible: true
            }
            PropertyChanges {
                target: viewportTools
                visible: false
            }
        },
        State {
            name: "writing"

            PropertyChanges {
                target: rotation
                angle: 180
            }

            PropertyChanges {
                target: listTools
                visible: false
            }
            PropertyChanges {
                target: viewportTools
                visible: true
            }
        }
    ]
    transitions: [
        Transition {
            id: transition
            SequentialAnimation {
                PauseAnimation {
                    duration: 50
                }

                PropertyAnimation {
                    target: rotation
                    property: "angle"
                    duration: 150
                }
            }
        }
    ]
}
/*##^##
Designer {
    D{i:0}D{i:19;transitionDuration:2000}
}
##^##*/

