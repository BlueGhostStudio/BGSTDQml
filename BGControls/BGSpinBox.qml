import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.SpinBox {
    id: control

    readonly property var indicatorWidth: contentItem.width

    // Note: the width of the indicators are calculated into the padding
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentItem.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             up.implicitIndicatorHeight, down.implicitIndicatorHeight)

    leftPadding: padding + (control.mirrored ? (up.indicator ? up.indicator.width : 0) : (down.indicator ? down.indicator.width : 0))
    rightPadding: padding + (control.mirrored ? (down.indicator ? down.indicator.width : 0) : (up.indicator ? up.indicator.width : 0))

    validator: IntValidator {
        locale: control.locale.name
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }

    contentItem: TextField {
        z: 2

        BGControls.options: {
            "transparent": !control.editable
        }

        text: control.displayText

        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: control.inputMethodHints

        /*BGPaletteValue on opacity {
            targetControl: control.contentItem
            paletteProperty: "custom"
            callback: function () {
                return 1
            }
        }*/
    }

    up.indicator: BGBorderBackground{
        x: control.mirrored ? 0 : control.width - width
        width: height
        height: control.implicitContentHeight
        /*color: control.BGPalette.mixColor("button", true, false, up.hovered,
                                       up.pressed,BGPalette.UNSET,
                                       control.BGControls.background)*/
        BGPaletteValue on color {
            targetControl: control.up
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS & ~BGPaletteValue.STM_HIGHLIGHT
        }

        shape: BGBorderBackground.RoundedRectangle
        border: control.up.pressed ? false : true

        ColorImage {
            anchors.centerIn: parent
            color: BGColor.contrast(parent.color)
            source: "icons/add.png"
            width: 20; height: 20
        }
        BGPaletteValue on opacity {
            targetControl: control.up.indicator
            paletteProperty: "enable"
            section: "default"
        }
    }

    down.indicator: BGBorderBackground{
        x: control.mirrored ? control.width - width : 0
        width: height
        height: control.implicitContentHeight

        BGPaletteValue on color {
            targetControl: control.down
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS & ~BGPaletteValue.STM_HIGHLIGHT
        }
        shape: BGBorderBackground.RoundedRectangle
        border: control.down.pressed ? false : true

        ColorImage {
            anchors.centerIn: parent
            color: BGColor.contrast(parent.color)
            source: "icons/remove.png"
            width: 20; height: 20
        }

        BGPaletteValue on opacity {
            targetControl: control.down.indicator
            paletteProperty: "enable"
            section: "default"
        }
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "input"
    }
}
