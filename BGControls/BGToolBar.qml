import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.ToolBar {
    id: control

    default property alias items: rlToolbarLayout.data

    BGControls.isContainer: true

    spacing: 3
    leftPadding: spacing
    rightPadding: spacing
    topPadding: spacing
    bottomPadding: spacing

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    BGControls.role: control.position === T.TabBar.Header ? BGControls.PRIMARY : BGControls.ACCENT
    BGControls.shade: control.position === ToolBar.Footer ? -8 : 0

    RowLayout {
        id: rlToolbarLayout
        anchors.fill: parent
        spacing: control.spacing
    }

    background: Rectangle {
        implicitHeight: 40
        //color: control.BGControls.background
        BGPaletteValue on color {
            targetControl: control
            section: "panel"
        }
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "default"
    }
}
