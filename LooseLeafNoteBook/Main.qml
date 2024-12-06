import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

import BGStudio.Controls
import BGStudio.Handwriter

ApplicationWindow {
    id: window
    width: 640
    height: 480

    visible: true

    /*LLPs.MainForm {
        anchors.fill: parent
    }*/

    LooseLeafNoteBookStackView {
        anchors.fill: parent
    }

    Connections {
        target: Application

        function onStateChanged() {
            if (Application.state === Qt.ApplicationActive)
                AndroidInterface.setFullSreen()
        }
    }
}
