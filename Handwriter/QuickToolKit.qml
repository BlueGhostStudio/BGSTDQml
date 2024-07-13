import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

Popup {
    id: toolKit

    readonly property HWViewport viewport: HWViewport.viewport
    property list<real> zoomLevel: [1, 3.3, 6.6, 10]
    default property alias toolKits: clToolKit.children

    // edge: Qt.BottomEdge
    width: Math.min(viewport.width - 16, viewport.height)
    x: (viewport.width - width) / 2
    y: viewport.height
    z: 10

    dim: false
    modal: false
    closePolicy: Popup.CloseOnPressOutside

    topPadding: 12
    leftPadding: 12
    rightPadding: 12
    bottomPadding: 12

    Timer {
        id: timer
        interval: 2000
        running: false
        onTriggered: toolKit.close()
    }

    ButtonGroup {
        id: buttonGroup
        exclusive: true
    }

    Image {
        parent: viewport

        x: viewport.lastPos.x - width / 2
        y: viewport.lastPos.y - height / 2
        z: 2

        width: 22
        height: 22

        visible: toolKit.opened

        source: "qrc:/qt/qml/BGStudio/Controls/icons/target.png"
    }

    contentItem: MouseArea {
        implicitHeight: clToolKit.implicitHeight
        propagateComposedEvents: true
        ColumnLayout {
            id: clToolKit
            anchors.fill: parent
            z: -1
            RowLayout {
                Layout.alignment: Qt.AlignHCenter

                Repeater {
                    id: repZoomLevel
                    model: toolKit.zoomLevel

                    ToolButton {
                        Layout.fillWidth: true
                        text: modelData

                        checked: viewport.zoomRatio === modelData
                        checkable: true
                        ButtonGroup.group: buttonGroup

                        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/magnifier.png"

                        onClicked: {
                            const pos = viewport.lastPos
                            viewport.zoom(modelData, pos.x, pos.y, true)
                        }
                    }
                }
            }
            UserSavedStrokes {
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                writer: viewport.paper ? viewport.paper.canvas : null
            }
        }

        onPressed: (mouse) => {
                       timer.restart()
                       mouse.accepted = false
                   }
    }

    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
        NumberAnimation {
            property: "y"
            easing.type: Easing.OutBack
            to: viewport.height - implicitHeight - 8
        }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0 }
        NumberAnimation { property: "y"; to: viewport.height }
    }

    onOpened: timer.restart()
    onClosed: timer.stop()

    Connections {
        target: viewport.paper ? viewport.paper.canvas : null

        function onPressAndHold() {
            toolKit.open()
        }
        function onHoldAndMove() {
            toolKit.close()
        }
    }
}
