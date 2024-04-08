import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.ScrollBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 2
    visible: control.policy !== T.ScrollBar.AlwaysOff
    minimumSize: orientation === Qt.Horizontal ? height / width : width / height

    contentItem: BGBorderBackground {
        implicitWidth: control.interactive ? 8 : 4
        implicitHeight: control.interactive ? 8 : 4

        cornerRadius: control.interactive ? 4 : 2
        shape: BGBorderBackground.RoundedRectangle
        border: control.interactive
        //color: control.pressed ? control.palette.dark : control.palette.mid
        /*color: control.BGPalette.btnColor(control.BGControls.background, true,
                                          control.hovered, false,
                                          control.pressed)*/
        /*color: control.BGPalette.mixColor("default", false, true, control.hovered,
                                       control.pressed, BGPalette.UNSET,
                                       control.BGControls.background)*/
        BGPaletteValue on color {
            targetControl: control
            section: "default"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS | BGPaletteValue.STM_FORCE_HIGHLIGHT
        }

        opacity: 0.0

        states: State {
            name: "active"
            when: control.policy === T.ScrollBar.AlwaysOn || (control.active && control.size < 1.0)
            PropertyChanges { control.contentItem.opacity: 0.75 }
        }

        transitions: Transition {
            from: "active"
            SequentialAnimation {
                PauseAnimation { duration: 450 }
                NumberAnimation { target: control.contentItem; duration: 200; property: "opacity"; to: 0.0 }
            }
        }
    }
}
