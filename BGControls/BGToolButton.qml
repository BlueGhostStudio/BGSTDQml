import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.ToolButton {
    id: control

    flat: true

    icon.width: 22
    icon.height: 22

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 2

    horizontalPadding: padding + 2

    contentItem: BGButton.contentItem

    /*BGPalette.section: "button"
    BGPalette.stateMask: BGPalette.STM_ALL_EXCEPT_FOCUS*/

    background: BGBorderBackground {
        x: 1
        y: 1
        width: control.width - 2
        height: control.height - 2

        border: (control.down || control.checked || control.flat) ? false : true
        visible: !control.flat
        //color: control.BGPalette.color

        BGPaletteValue on color {
            targetControl: control
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS & ~BGPaletteValue.STM_BACKGROUND
        }

        shape: BGBorderBackground.RoundedRectangle
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "button"
    }
}
