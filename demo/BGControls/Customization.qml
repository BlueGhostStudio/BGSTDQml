import QtQuick
import QtQuick.Layouts

import BGStudio.Controls

FieldsGroupBox {
    id: flCustomization
    property ApplicationWindow mainWin

    BGControls.options: {
        "transparent": true,
        "noTitleBackground": true
    }

    title: "Preferences"

    FieldItem {
        field: "dark"
        value: control.checked
        fillWidth: false
        initialValue: mainWin.BGControls.theme === BGControls.BRIGHTNESS_INVERSION

        Switch {
            text: mainWin.BGControls.dark ? "DARK" : "LIGHT"

            onCheckedChanged: {
                mainWin.BGControls.theme = checked
                        ? BGControls.BRIGHTNESS_INVERSION
                        : BGControls.NORMAL
            }
        }
    }

    FieldItem {
        field: "baseColor"
        fillWidth: true
        ColorMixer {
            // FieldItem.fillWidth: true
            title: "Base Color"

            onColorChanged: mainWin.BGControls.base = color
        }
    }

    FieldItem {
        fillWidth: true
        field: "primaryColor"
        ColorMixer {
            // FieldItem.fillWidth: true
            title: "Primary Color"

            onColorChanged: mainWin.BGControls.primary = color
        }
    }

    FieldItem {
        fillWidth: true
        field: "accentColor"
        ColorMixer {
            // FieldItem.fillWidth: true
            title: "Accent Color"

            onColorChanged: mainWin.BGControls.accent = color
        }
    }

    FieldItem {
        label: "Shade"
        Slider {
            from: 0
            to: 100
            onMoved: mainWin.BGControls.shade = value
        }
    }
    FieldItem {
        label: "Hue"
        Slider {
            from: 0
            to: 360

            onMoved: mainWin.BGControls.hue = value
        }
    }

    Component.onCompleted: {
        fieldsLayout.control("baseColor").color =
                mainWin.BGControls.base
        fieldsLayout.control("primaryColor").color =
                mainWin.BGControls.primary
        fieldsLayout.control("accentColor").color =
                mainWin.BGControls.accent
    }
}
