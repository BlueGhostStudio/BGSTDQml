import QtQuick
import QtQuick.Controls as QC
import BGStudio.Controls

QC.ApplicationWindow {
    id: control

    BGControls.isContainer: true
    //BGControls.covered: true

    background: Rectangle {
        color: control.BGControls.background
    }

    Component.onCompleted: BGRefreshConnection.refreshStarted()
}
