import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

ComboBox {
    id: colorSelector
    property alias strokeColor: colorSelector.currentIndex
    property HWWriter writer

    model: 7
    contentItem: Item {
        implicitWidth: 32
        implicitHeight: 32

        Rectangle {
            x: 5; y: 5
            width: parent.width - 5
            height: 22
            color: HWSettings.palette[colorSelector.currentIndex]
        }
    }
    delegate: ItemDelegate {
        required property var model
        required property int index

        width: ListView.view.width
        height: 32

        highlighted: colorSelector.highlightedIndex === index
        hoverEnabled: colorSelector.hoverEnabled

        contentItem: Rectangle {
            color: HWSettings.palette[index]
        }
    }

    Component.onCompleted:  {
        if (writer) {
            strokeColorChanged.connect(
                        () => {
                            writer.strokeColor = strokeColor
                        })
        }
    }
}
