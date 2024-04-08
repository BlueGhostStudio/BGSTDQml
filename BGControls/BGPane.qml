import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.Pane {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    BGControls.options:  QtObject {
        property bool transparent: true
        property bool shade: false
    }

    BGControls.isContainer: true

    padding: 6

    background: Rectangle {
        BGPaletteValue on color {
            targetControl: control
            section: control.BGControls.options.transparent ?
                         (control.BGControls.options.shade ?
                              "panel" :
                              "panel/VAR_NOSHADE") :
                         "panel/VAR_SHADE"
        }
    }

    //opacity: BGPalette.enabledOpacity
    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "panel"
    }
}
