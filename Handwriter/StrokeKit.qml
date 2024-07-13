import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

ColumnLayout {
    id: strokeKit

    property HWWriter writer
    property alias strokePresets: strokePresets
    property alias userSavedStrokes: userSavedStrokes
    property alias strokeColorSelector: strokeColorSelector
    property alias strokeAdjustment: strokeAdjustment
    property alias strokePreview: strokePreview

    spacing: 0

    readonly property var globalGroupBoxOptions: {
        "border": GroupBox.NoBorder,
        "noTitleBackground": true,
        "level": 2
    }

    TabBar {
        id: tabBar
        Layout.fillWidth: true
        BGControls.role: BGControls.BASE

        TabButton {
            text: "Quick"
        }
        TabButton {
            text: "Adjustment"
        }
    }

    SwipeView {
        Layout.fillWidth: true
        Layout.fillHeight: true

        currentIndex: tabBar.currentIndex

        interactive: false

        contentItem.clip: true

        FieldsPane {
            FieldItem {
                FieldsGroupBox {
                    BGControls.options: globalGroupBoxOptions

                    title: "Strokes"

                    FieldItem {
                        label: "Presets"
                        orientation: Qt.Vertical
                        StrokePresets {
                            id: strokePresets
                            writer: strokeKit.writer
                        }
                    }

                    FieldItem {
                        label: "User saved"
                        orientation: Qt.Vertical
                        UserSavedStrokes {
                            id: userSavedStrokes

                            writer: strokeKit.writer

                            /*onRequesSave: {
                                saveStroke(strokeAdjustment.options)
                            }*/
                        }
                    }
                }
            }

            FieldItem {
                FieldsGroupBox {
                    BGControls.options: globalGroupBoxOptions

                    title: "Palette"
                    FieldItem {
                        StrokeColorSelector {
                            id: strokeColorSelector
                            writer: strokeKit.writer
                        }
                    }
                }
            }
        }

        ColumnLayout {
            spacing: 0

            StrokePreview {
                id: strokePreview

                clip: true

                Layout.fillWidth: true
                implicitHeight: width

                /*writer.strokeType: strokeKit.writer.strokeType
                writer.minPenSize: strokeKit.writer.minPenSize
                writer.strokeSize: strokeKit.writer.strokeSize
                writer.fadeoutLimit: strokeKit.writer.fadeoutLimit
                writer.fadeinLimit: strokeKit.writer.fadeinLimit
                writer.velocityThreshold: strokeKit.writer.velocityThreshold*/

                Connections {
                    target: strokeKit
                    function onWriterChanged() {
                        if (strokeKit.writer) {
                            strokeKit.writer.strokeType = strokeKit.writer.strokeType
                            strokeKit.writer.minPenSize = strokeKit.writer.minPenSize
                            strokeKit.writer.strokeSize = strokeKit.writer.strokeSize
                            strokeKit.writer.fadeoutLimit = strokeKit.writer.fadeoutLimit
                            strokeKit.writer.fadeinLimit = strokeKit.writer.fadeinLimit
                            strokeKit.writer.velocityThreshold = strokeKit.writer.velocityThreshold
                        }
                    }
                }
            }

            FieldsPane {
                id: fieldsGroupBox

                Layout.fillWidth: true
                Layout.fillHeight: true

                flickable.interactive: !strokePreview.writer.writing
                                       && !strokePreview.isGestureInProgress

                FieldItem {
                    StrokeAdjustment {
                        id: strokeAdjustment

                        writer: strokeKit.writer

                        BGControls.options: globalGroupBoxOptions
                    }
                }
            }
        }
    }
}
