import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

Button {
    id: control

    property int strokeColor: 0

    checkable: true

    Layout.fillWidth: true
    contentItem: Rectangle {
        implicitHeight: 22
        implicitWidth: 22
        color: HWSettings.palette[index]
    }
}
