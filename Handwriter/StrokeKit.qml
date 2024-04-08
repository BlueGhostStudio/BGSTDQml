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
    property alias strokeSettings: strokeSettings
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
                            onClicked: {
                                strokeSettings.loadSettings(options)
                                userSavedStrokes.buttonGroup.checkState = Qt.Unchecked
                            }
                        }
                    }

                    FieldItem {
                        label: "User saved"
                        orientation: Qt.Vertical
                        UserSavedStrokes {
                            id: userSavedStrokes

                            Menu {
                                id: userSavedStrokesMenu
                                property int index

                                MenuItem {
                                    text: "Update"

                                    onTriggered: {
                                        userSavedStrokes.updateStroke(userSavedStrokesMenu.index,
                                                               strokeSettings.options)
                                    }
                                }

                                MenuItem {
                                    text: "Remove"

                                    onTriggered: {
                                        userSavedStrokes.removeStroke(userSavedStrokesMenu.index)
                                    }
                                }
                            }

                            onClicked: {
                                strokeSettings.loadSettings(options)
                                strokePresets.buttonGroup.checkState = Qt.Unchecked
                            }
                            onPressAndHold: (index) => {
                                                userSavedStrokesMenu.index = index
                                                userSavedStrokesMenu.open()
                                            }

                            onRequesSave: {
                                saveStroke(strokeSettings.options)
                            }
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
                            onClicked: function(index) {
                                if (index !== strokeColor)
                                    userSavedStrokes.buttonGroup.checkState = Qt.Unchecked
                            }

                            onStrokeColorChanged: {
                                if (strokeSettings.strokeColor !== strokeColor)
                                    strokeSettings.strokeColor = strokeColor
                            }

                            // writer: strokeKit.writer
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

                writer.strokeType: strokeSettings.options.strokeType
                writer.minPenSize: strokeSettings.options.minPenSize
                writer.strokeSize: strokeSettings.options.strokeSize
                writer.fadeoutLimit: strokeSettings.options.fadeoutLimit
                writer.fadeinLimit: strokeSettings.options.fadeinLimit
                writer.velocityThreshold: strokeSettings.options.velocityThreshold
            }

            FieldsPane {
                id: fieldsGroupBox

                Layout.fillWidth: true
                Layout.fillHeight: true

                flickable.interactive: !strokePreview.writer.writing
                                       && !strokePreview.gestureArea.isGestureInProgress

                FieldItem {
                    StrokeSettings {
                        id: strokeSettings

                        writer: strokeKit.writer

                        BGControls.options: globalGroupBoxOptions

                        onStrokeColorChanged: strokeColorSelector.strokeColor = strokeColor
                        onActivated: {
                            strokePresets.buttonGroup.checkState = Qt.Unchecked
                            userSavedStrokes.buttonGroup.checkState = Qt.Unchecked
                        }
                    }
                }
            }
        }
    }
}
