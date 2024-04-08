import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.Drawer {
    id: control

    parent: T.Overlay.overlay

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    topPadding: control.edge === Qt.BottomEdge ? 3 : 0
    leftPadding: control.edge === Qt.RightEdge ? 3 : 0
    rightPadding: control.edge === Qt.LeftEdge ? 3 : 0
    bottomPadding: control.edge === Qt.TopEdge ? 3 : 0

    enter: Transition { SmoothedAnimation { velocity: 5 } }
    exit: Transition { SmoothedAnimation { velocity: 5 } }

    BGControls.isContainer: true

    background: Rectangle {
        BGPaletteValue on color {
            targetControl: control
            section: "panel/VAR_POPUP"
        }

        Rectangle {
            readonly property bool horizontal: control.edge === Qt.LeftEdge || control.edge === Qt.RightEdge
            width: horizontal ? 3 : parent.width
            height: horizontal ? parent.height : 3
            color: control.BGControls.accent
            x: control.edge === Qt.LeftEdge ? parent.width - 3 : 0
            y: control.edge === Qt.TopEdge ? parent.height - 3 : 0
        }
    }

    T.Overlay.modal: Rectangle {
        BGPaletteValue on color {
            targetControl: control
            paletteProperty: "custom"
            callback: () => {
                          return colorAdjustment("panel/VAR_POPUP/PART_OVERLAY/VAR_MODAL",
                                                 "background");
                      }
        }
    }

    T.Overlay.modeless: Rectangle {
        BGPaletteValue on color {
            targetControl: control
            paletteProperty: "custom"
            callback: () => {
                          return colorAdjustment("panel/VAR_POPUP/PART_OVERLAY/VAR_MODELESS",
                                                 "background")
                      }
        }
    }
}
