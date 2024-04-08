import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.Popup {
    id: control

    BGControls.isContainer: true

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    padding: 6

    background: BGBorderBackground {
        //color: control.BGControls.background
        BGPaletteValue on color {
            targetControl: control
            section: "panel"
        }

        shape: BGBorderBackground.RoundedRectangle
        cornerRadius: 6
    }

    readonly property color overlayColor: BGColor.brightnessInversion(control.BGControls.background, 0.75)

    T.Overlay.modal: Rectangle {
        color: Color.transparent(overlayColor, 0.5)
    }

    T.Overlay.modeless: Rectangle {
        color: Color.transparent(overlayColor, 0.12)
    }
}
