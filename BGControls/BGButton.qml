import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.Button {
    id: control

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

    background: BGBorderBackground {
        id: borderBackground
        border: (control.down || control.checked || control.flat) ? false : true
        visible: !control.flat
        shape: BGBorderBackground.RoundedRectangle

        BGPaletteValue on color {
            targetControl: control
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS
            //baseColor: control.BGControls.background
        }
    }

    //opacity: control.BGPalette.enabledOpacity
    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "button"
    }
}
