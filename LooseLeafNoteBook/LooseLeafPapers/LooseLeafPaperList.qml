import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BGStudio.Controls
import BGStudio.Handwriter

import LooseLeafNoteBook

ListView {
    id: looseLeafPapers

    property LooseLeafViewport viewport

    clip: true
    visible: true

    cacheBuffer: 10
    displayMarginBeginning: looseLeafPapers.width * 5
    displayMarginEnd: displayMarginBeginning

    model: HWDataStorage.papersModel

    snapMode: ListView.SnapToItem
    highlightRangeMode: ListView.StrictlyEnforceRange
    highlight: Item {
        Rectangle {
            anchors.fill: parent
            // anchors.margins: 8
            anchors.topMargin: 8
            anchors.bottomMargin: 8
            border.color: BGControls.accent
            radius: 8
        }
    }
    highlightMoveDuration: 50
    highlightMoveVelocity: -1
    highlightResizeDuration: 50
    highlightResizeVelocity: -1
    preferredHighlightBegin: (width - currentItem.width) / 2
    preferredHighlightEnd: width - preferredHighlightBegin

    orientation: ListView.Horizontal

    delegate:  Item {
        readonly property real sizeRatio: hwPaperPreview.implicitWidth
                                          / hwPaperPreview.implicitHeight
        width: (height - 32) * sizeRatio + 16
        height: looseLeafPapers.height

        HWPaperPreview {
            id: hwPaperPreview

            anchors.centerIn: parent

            width: parent.width - 16
            height: parent.height - 32

            MouseArea {
                anchors.fill: hwPaperPreview

                onClicked: looseLeafPapers.currentIndex = index
            }

            RowLayout {
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                spacing: 0

                ToolButton {

                    icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/enter.png"

                    visible: looseLeafPapers.currentIndex === index

                    onClicked: {
                        activePaperList.openPaper(model.display)
                    }
                }

                Rectangle {
                    width: 32
                    height: 32
                    x: parent.width - width - 8
                    y: 8

                    radius: 16
                    color: viewport && viewport.paper
                           && viewport.paper.localStorageID === model.display
                           ? BGControls.accent : BGControls.background

                    Text {
                        anchors.centerIn: parent
                        text: "#" + hwPaperPreview.localStorageID
                        color: BGColor.contrast(parent.color)
                    }
                }
            }

            Component.onCompleted: loadLocalStorageData(model.display)
        }
    }
}
