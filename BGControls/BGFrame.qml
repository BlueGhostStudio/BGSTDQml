import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.Frame {
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
            section: "panel/VAR_SHADE"
        }
        shape: BGBorderBackground.RoundedRectangle
    }

    //opacity: BGPalette.enabledOpacity
    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "panel"
    }
}
