import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.Switch {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 2

    indicator: BGBorderBackground {
        implicitWidth: 44
        implicitHeight: 22
        x: control.text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2

        shape: BGBorderBackground.RoundedRectangle
        style: BGBorderBackground.TwoDimensional
        cornerRadius: 11
        color: control.BGSwitch.stateColor
        borderWidth: 1

        BGBorderBackground {
            x: 2
            y: 2
            implicitWidth: indicatorBlock.x + 16
            implicitHeight: 18

            cornerRadius: 9
            shape: BGBorderBackground.RoundedRectangle
            border: false
            color: control.BGSwitch.stateColor
            opacity: control.visualPosition * 0.33
        }
        BGBorderBackground {
            id: indicatorBlock
            implicitWidth: 18
            implicitHeight: 18

            x: Math.max(
                   2, Math.min(
                       parent.width - width - 2,
                       control.visualPosition * parent.width - (width / 2)))
            y: parent.height / 2 - height / 2
            shape: BGBorderBackground.Ellipse
            color: control.checked ? control.BGSwitch.stateColor
                                   : BGColor.brightnessInversion(
                                         control.BGControls.background, 0.25)
        }
    }

    contentItem: T.Label {
        text: control.text
        BGText.textScale: BGText.Button
        font: BGText.font
        color: control.BGSwitch.stateColor

        verticalAlignment: Text.AlignVCenter

        leftPadding: control.indicator
                     && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator
                      && control.mirrored ? control.indicator.width + control.spacing : 0
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "button"
    }
}
