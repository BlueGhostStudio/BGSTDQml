import QtQuick
import QtQuick.Layouts

import BGStudio.Controls

FieldItem {
    property alias model: repeater.model
    property alias columns: gridLayout.columns

    property Component ctrlCmp

    fillWidth: false

    GridLayout {
        id: gridLayout

        anchors.centerIn: parent

        columns: 2
        columnSpacing: 0
        rowSpacing: 0

        Repeater {
            id: repeater
            model: []
            Loader {
                sourceComponent: ctrlCmp
                onLoaded: {
                    var data = modelData
                    for (let x in data)
                        item[x] = data[x]
                }
            }
        }
    }
}
