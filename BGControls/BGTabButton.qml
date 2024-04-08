import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.TabButton {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 6

    icon.width: 22
    icon.height: 22
    icon.color: checked ? control.palette.windowText : control.palette.brightText

    contentItem: control.BGButton.contentItem

    background: BGBorderBackground {
        border: (control.down || !control.checked) ? false : true
        visible: !control.flat
        BGPaletteValue on color {
            targetControl: control
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS
        }

        shape: BGBorderBackground.RoundedRectangle
    }
}

