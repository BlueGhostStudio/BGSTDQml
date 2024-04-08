import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

Flow {
    id: strokeColorSelector
    property int strokeColor: buttonGroup.buttons[0].strokeColor
    property HWWriter writer

    signal clicked(int index)

    ButtonGroup {
        id: buttonGroup
        exclusive: true

        onClicked: function (button) {
            if (strokeColorSelector.strokeColor !== button.strokeColor)
                strokeColorSelector.strokeColor = button.strokeColor
        }
    }

    Repeater {
        model: 7

        StrokeColorButton {
            strokeColor: index
            ButtonGroup.group: buttonGroup

            onClicked: strokeColorSelector.clicked(index)
        }
    }

    Component.onCompleted: {
        buttonGroup.buttons[0].checked = true

        strokeColorChanged.connect(
                    function () {
                        var strokeColorButton = buttonGroup.buttons[strokeColor];
                        if (!strokeColorButton.checked)
                            strokeColorButton.checked = true
                    })

        if (writer) {
            strokeColorChanged.connect(
                        function () {
                            var strokeColorButton = buttonGroup.buttons[strokeColor];
                            if (!strokeColorButton.checked)
                                strokeColorButton.checked = true

                            writer.strokeColor = strokeColor
                        })
        }
    }
}
