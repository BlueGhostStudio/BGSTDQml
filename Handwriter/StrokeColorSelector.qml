import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

Flow {
    id: strokeColorSelector

    property HWWriter writer

    signal clicked(int index)

    ButtonGroup {
        id: buttonGroup
        exclusive: true

        onClicked: function (button) {
            if (writer)
                writer.strokeColor = button.strokeColor
        }
    }

    Repeater {
        model: 7

        StrokeColorButton {
            strokeColor: index
            ButtonGroup.group: buttonGroup

            checked: writer ? writer.strokeColor === index : false
        }
    }

    Component.onCompleted: {}
}
