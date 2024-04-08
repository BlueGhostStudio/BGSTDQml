import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.ScrollIndicator {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 2

    contentItem: BGBorderBackground {
        implicitWidth: 8
        implicitHeight: 8

        BGPaletteValue on color {
            targetControl: control
            section: "default"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS | BGPaletteValue.STM_FORCE_HIGHLIGHT
        }

        visible: control.size < 1.0
        opacity: 0.0

        cornerRadius: 4
        shape: BGBorderBackground.RoundedRectangle

        states: State {
            name: "active"
            when: control.active
            PropertyChanges { control.contentItem.opacity: 0.75 }
        }

        transitions: [
            Transition {
                from: "active"
                SequentialAnimation {
                    PauseAnimation { duration: 450 }
                    NumberAnimation { target: control.contentItem; duration: 200; property: "opacity"; to: 0.0 }
                }
            }
        ]
    }
}
