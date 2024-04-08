import QtQuick
import QtQuick.Layouts

import BGStudio.Controls

GroupBox {
    id: groupBox

    BGControls.options: {
        "noTitleBackground": true
    }

    component PreviewGroupBox: GroupBox {
        id: gbPreview

        default property Item previewItem

        topPadding: implicitLabelHeight + 4
        leftPadding: 3
        rightPadding: 3
        bottomPadding: 3

        title: "Preview"

        BGControls.options: {
            "transparent": true,
            "levelCounter": false,
            "shade": true
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 1

            ToolBar {
                Layout.fillWidth: true

                spacing: 1

                ColorSelectorButton {
                    onColorMixed: gbPreview.BGControls.base = color

                    Component.onCompleted: color = BGControls.base
                }

                ColorSelectorButton {
                    onColorMixed: gbPreview.BGControls.primary = color

                    Component.onCompleted: color = BGControls.primary
                }

                ColorSelectorButton {
                    onColorMixed: gbPreview.BGControls.accent = color

                    Component.onCompleted: color = BGControls.accent
                }

                Item { Layout.fillWidth: true }

                Switch {
                    text: gbPreview.BGControls.dark ? "DARK" : "LIGHT"

                    onCheckedChanged: {
                        gbPreview.BGControls.theme = checked
                                ? BGControls.BRIGHTNESS_INVERSION
                                : BGControls.NORMAL
                    }
                }
            }
            ToolBar {
                Layout.fillWidth: true

                BGControls.role: BGControls.BASE

                Text {
                    Layout.fillWidth: false
                    text: "Shade"
                }

                Slider {
                    Layout.fillWidth: true

                    from: 0
                    to: 100

                    onMoved: gbPreview.BGControls.shade = value
                }

                Text {
                    Layout.fillWidth: false
                    text: "Hue"
                }

                Slider {
                    Layout.fillWidth: true

                    from: 0
                    to: 360

                    onMoved: gbPreview.BGControls.hue = value
                }
            }

            Item {
                id: itPreviewWrap
                Layout.fillWidth: true
            }
        }

        Component.onCompleted: {
            if (previewItem) {
                previewItem.parent = itPreviewWrap
                previewItem.anchors.fill = itPreviewWrap
                previewItem.anchors.margins = 6
                itPreviewWrap.implicitHeight = previewItem.implicitHeight + 12
            }
        }
    }

    contentItem: Flickable {
        id: flickable
        clip: true
        contentWidth: topPreview.implicitWidth
        contentHeight: topPreview.implicitHeight

        flickableDirection: Flickable.AutoFlickIfNeeded
        boundsMovement: Flickable.StopAtBounds
        boundsBehavior: Flickable.DragAndOvershootBounds

        ScrollBar.vertical: ScrollBar {
            parent:  groupBox
            anchors.right: parent.right
            anchors.top: flickable.top
            anchors.bottom: flickable.bottom
        }
        ScrollBar.horizontal: ScrollBar {
            parent:  groupBox
            anchors.bottom: parent.bottom
            anchors.left: flickable.left
            anchors.right: flickable.right
        }

        PreviewGroupBox {
            id: topPreview

            width: Math.max(groupBox.contentItem.width, implicitWidth)

            ColumnLayout {
                Frame {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    contentItem: Flow {
                        Button { text: "Button"; highlighted: true }
                        ToolButton { text: "ToolButton" }
                        CheckBox { text: "CheckBox" }
                        RadioButton { text: "RadioButton" }
                    }
                }

                PreviewGroupBox {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    PreviewGroupBox {
                    }
                }

            }
        }
    }

}
