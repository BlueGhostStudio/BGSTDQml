import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls
import QtQuick.Layouts

Flow {
    id: strokeCollection
    /*property var writeParameter: buttonGroup.checkedButton ? buttonGroup.checkedButton.writeParameter
                                                    : undefined*/
    property int method

    property HWWriter writer

    // property ListModel collectionModel: ListModel {}
    property alias collectionModel : collectionModel
    property alias buttonGroup: buttonGroup

    spacing: 6

    signal clicked(int index, var writeParameter)
    signal pressAndHold(int index, var writeParameter)

    ListModel {
        id: collectionModel
    }

    ButtonGroup {
        id: buttonGroup
        exclusive: true
    }

    Repeater {
        id: repPresets
        model: collectionModel

        StrokeSampleButton {
            id: button

            required property var modelData
            required property int index

            writeParameter: modelData//collectionModel.get(index)
            ButtonGroup.group: buttonGroup

            onPressAndHold: strokeCollection.pressAndHold(index, writeParameter)
            onClicked: {
                strokeCollection.clicked(index, writeParameter)
                if (writer)
                    writer.writeParameter = writeParameter
                else
                    writer.writeParameterChanged(HWWriter.Custom, -1)
            }
        }

        /*function resetCurrentStroke(writeParameter) {
            repPresets.model.setProperty(currentIndex, "writeParameter", writeParameter)
        }*/
    }

    Connections {
        target: writer || null
        function onWriteParameterChanged(method, index) {
            if (strokeCollection.method === method
                    && index < buttonGroup.buttons.length) {
                buttonGroup.buttons[index].checked = true
            } else
                buttonGroup.checkState = Qt.Unchecked
        }
        function onStrokeAdjusted() {
            buttonGroup.checkState = Qt.Unchecked
        }
        function onStrokeColorChanged() {
            if (method !== HWWriter.Preset)
                buttonGroup.checkState = Qt.Unchecked
        }
    }

    Component.onCompleted: {
        /*if (writer) {
            writer.writeParameterChanged.connect(
                        (method, index) => {
                            if (strokeCollection.method === method
                                && index < buttonGroup.buttons.length) {
                                buttonGroup.buttons[index].checked = true
                            } else
                                buttonGroup.checkState = Qt.Unchecked
                        })
            writer.strokeAdjusted.connect(
                        () => {
                            buttonGroup.checkState = Qt.Unchecked
                        })
            writer.strokeColorChanged.connect(
                        () => {
                            if (method !== HWWriter.Preset)
                                buttonGroup.checkState = Qt.Unchecked
                        })
        }*/
    }

    /*Component.onCompleted: {
        if (writer) {
            strokeChanged.connect(
                        () => {
                            writer.loadSettings(options)
                        })
        }
    }*/
}
