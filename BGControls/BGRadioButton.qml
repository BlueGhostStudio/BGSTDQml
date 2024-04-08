import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.RadioButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 2

    indicator: BGBorderBackground {
        implicitWidth: 20
        implicitHeight: 20
        x: control.text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2
        shape: BGBorderBackground.Ellipse
        style: BGBorderBackground.TwoDimensional
        cornerRadius: 6
        borderWidth: control.checked ? 1 : 2
        color: control.BGSwitch.stateColor

        BGBorderBackground {
            implicitWidth: 16
            implicitHeight: 16
            visible: control.checked

            x: 2
            y: 2

            shape: BGBorderBackground.Ellipse
            color: control.BGSwitch.stateColor
        }
    }

    contentItem: T.Label {
        text: control.text
        BGText.textScale: BGText.Button
        font: BGText.font
        color: control.BGSwitch.stateColor

        verticalAlignment: Text.AlignVCenter

        leftPadding: control.indicator.width + control.spacing
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "button"
    }
}
