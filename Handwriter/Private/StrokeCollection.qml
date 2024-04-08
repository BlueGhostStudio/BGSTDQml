import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls

Flow {
    id: strokeCollection
    property var options: buttonGroup.checkedButton ? buttonGroup.checkedButton.options
                                                    : undefined
    property HWWriter writer

    property ListModel collectionModel: ListModel {}
    property alias buttonGroup: buttonGroup

    spacing: 6

    signal clicked(int index)
    signal pressAndHold(int index)

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

            options: modelData
            ButtonGroup.group: buttonGroup

            onPressAndHold: strokeCollection.pressAndHold(index)
            onClicked: strokeCollection.clicked(index)
        }

        function resetCurrentStroke(options) {
            repPresets.model.setProperty(currentIndex, "options", options)
        }
    }

    Component.onCompleted: {
        if (writer) {
            strokeChanged.connect(
                        () => {
                            writer.loadSettings(options)
                        })
        }
    }
}
