import QtQuick
import QtQuick.Controls as QC
import BGStudio.Controls
import QtQuick.Effects

QC.RoundButton {
    id: control

    icon.width: 40
    icon.height: 40

    spacing: 6
    padding: 6

    horizontalPadding: padding + 2
    verticalPadding: padding + 2

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    contentItem: BGButton.contentItem

    // BGPalette.section: "button"
    // BGPalette.stateMask: BGPalette.STM_ALL_EXCEPT_FOCUS

    background: BGBorderBackground {
        //border: (control.down || control.checked || control.flat) ? false : true
        visible: !control.flat
        // color: control.BGPalette.color
        BGPaletteValue on color {
            targetControl: control
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS
        }

        shape: BGBorderBackground.Ellipse

        implicitWidth: 48
        implicitHeight: 48

        layer.enabled: true
        layer.effect: MultiEffect {
            autoPaddingEnabled: true
            shadowEnabled: true
            shadowBlur: !control.down && !control.checked ? 1 : 0
            shadowHorizontalOffset: !control.down && !control.checked ? 5 : 0
            shadowVerticalOffset:  !control.down && !control.checked ? 5 : 0
            shadowOpacity:  !control.down && !control.checked ? 0.5 : 0
        }
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "button"
    }
}
