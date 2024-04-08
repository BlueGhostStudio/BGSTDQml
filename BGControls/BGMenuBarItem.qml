import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.MenuBarItem {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 2
    leftPadding: 6
    rightPadding: 6
    spacing: 2

    icon.width: 16
    icon.height: 16

    BGButton.isDelegate: true
    BGButton.labelAlignment: Qt.AlignLeft

    contentItem: control.BGButton.contentItem

    background: BGBorderBackground {
        implicitWidth: 20
        implicitHeight: 20

        border: false
        /*color: control.BGPalette.mixColor("button", true, control.highlighted || control.checked,
                                       control.hovered, control.down, BGPalette.UNSET,
                                       control.BGControls.background)*/
        BGPaletteValue on color {
            targetControl: control
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS & ~BGPaletteValue.STM_BACKGROUND
        }

        shape: BGBorderBackground.RoundedRectangle
        visible: control.down || control.highlighted
    }

    //opacity: control.BGPalette.enabledOpacity
    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "button"
    }
}
