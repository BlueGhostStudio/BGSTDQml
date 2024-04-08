import QtQuick
import QtQuick.Controls as QC
import BGStudio.Controls

QC.Page {
    id: control

    BGControls.isContainer: true

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding,
                            implicitHeaderWidth,
                            implicitFooterWidth)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding
                             + (implicitHeaderHeight > 0 ? implicitHeaderHeight + spacing : 0)
                             + (implicitFooterHeight > 0 ? implicitFooterHeight + spacing : 0))


    background: Rectangle {
        //color: control.BGControls.background
        BGPaletteValue on color {
            targetControl: control
            section: "pane"
        }
    }
}
