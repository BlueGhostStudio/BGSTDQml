import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.ItemDelegate {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 2

    icon.width: 16
    icon.height: 16

    BGButton.isDelegate: true

    // contentItem: IconLabel {
    //     spacing: control.spacing
    //     mirrored: control.mirrored
    //     display: control.display
    //     alignment: control.display === IconLabel.IconOnly || control.display === IconLabel.TextUnderIcon ? Qt.AlignCenter : Qt.AlignLeft

    //     icon: control.icon
    //     text: control.text
    //     font: control.font
    //     color: control.highlighted ? Fusion.highlightedText(control.palette) : control.palette.text
    // }

    contentItem: control.BGButton.contentItem

    background: BGBorderBackground {
        implicitWidth: 100
        border: false
        BGPaletteValue on color {
            targetControl: control
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS & ~BGPaletteValue.STM_BACKGROUND
        }

        shape: BGBorderBackground.RoundedRectangle
        visible: control.down || control.hovered || control.highlighted
    }
}
