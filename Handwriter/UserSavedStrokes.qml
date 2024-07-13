import QtQuick
import BGStudio.Controls
import BGStudio.Handwriter

import "qrc:/qt/qml/BGStudio/Handwriter/Private" as P

P.StrokeCollection {
    method: HWWriter.UseSaved

    Menu {
        id: userSavedStrokesMenu
        property int index

        MenuItem {
            text: "Update"

            onTriggered: {
                updateStroke(userSavedStrokesMenu.index)
            }
        }

        MenuItem {
            text: "Remove"

            onTriggered: {
                removeStroke(userSavedStrokesMenu.index)
            }
        }
    }

    onPressAndHold: (index) => {
                        userSavedStrokesMenu.parent = buttonGroup.buttons[index]
                        userSavedStrokesMenu.index = index
                        userSavedStrokesMenu.open()
                    }

    // signal requesSave()

    Button {
        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/add.png"

        onClicked: saveStroke()
    }

    function saveStroke() {
        // console.log(JSON.stringify(writer.writeParameter))
        if (writer) {
            writer.saveUserSavedWriteParameter(writer.writeParameter)
        }
        /*HWSettings.saveUserStroke(strokeOptions)
        var button = buttonGroup.buttons[collectionModel.count - 1]
        button.checked = true
        button.clicked()*/
    }

    function updateStroke(index) {
        if (writer) {
            writer.updateUserSavedWriteParameter(index, writer.writeParameter)
        }
    }

    function removeStroke(index) {
        if (writer) {
            writer.removeUserSavedWriteParameter(index)
            /*console.log("------", index, this, collectionModel.count)
            collectionModel.remove(index)*/
        }

        /*collectionModel.remove(index)
        HWSettings.removeUserStroke(index)*/
    }

    onWriterChanged: {
        if (writer && writer.writeParameterMethod === HWWriter.UseSaved) {
            var index = writer.writeParameterIndex
            if (index >= 0 && index < buttonGroup.buttons.length)
                buttonGroup.buttons[index].checked = true
        } else
            buttonGroup.checkState = Qt.Unchecked
    }


    Connections {
        target: writer || null

        function onUserSavedWriteParameterUpdated(index, writeParameter) {
            collectionModel.set(index, writeParameter)
            writer.writeParameterChanged(HWWriter.UseSaved, index)
        }
        function onUserSavedWriteParameterSaved(index, writeParameter) {
            writeParameter.__index__ = index.toString()
            writeParameter.__method__ = HWWriter.UseSaved
            collectionModel.append(writeParameter)
            writer.writeParameterChanged(HWWriter.UseSaved, index)
        }
        function onUserSavedWriteParameterRemoved(index) {
            buttonGroup.checkState = Qt.Unchecked
            collectionModel.remove(index)
            writer.writeParameterChanged(HWWriter.Custom, -1)
        }
    }

    Component.onCompleted: {
        const collection = HWSettings.userSavedStrokes
        for (let x in collection) {
            const strokeOptions = collection[x]
            if (strokeOptions) {
                collectionModel.append(
                            {
                                "__method__": HWWriter.UseSaved,
                                "__index__": x,
                                "strokeColor": strokeOptions.strokeColor || 0,
                                "strokeType": strokeOptions.strokeType || 0,
                                "minPenSize": strokeOptions.minPenSize || 0.1,
                                "strokeSize": strokeOptions.strokeSize || 1.15,
                                "fadeoutLimit": strokeOptions.fadeoutLimit || 0.8,
                                "fadeinLimit": strokeOptions.fadeinLimit || 1.2,
                                "velocityThreshold": strokeOptions.velocityThreshold || 95
                            })
            } else {
                collectionModel.append(
                            {
                                "__method__": HWWriter.UseSaved,
                                "__index__": x,
                                "strokeColor": 0,
                                "strokeType": 0,
                                "minPenSize": 0.1,
                                "strokeSize": 1.15,
                                "fadeoutLimit": 0.8,
                                "fadeinLimit": 1.2,
                                "velocityThreshold": 95
                            })
            }
        }

        /*if (writer) {
            writer.userSavedWriteParameterUpdated.connect(
                        (index, writeParameter) => {
                            collectionModel.set(index, writeParameter)
                            writer.writeParameterChanged(HWWriter.UseSaved, index)
                        })
            writer.userSavedWriteParameterSaved.connect(
                        (index, writeParameter) => {
                            writeParameter.__index__ = index.toString()
                            writeParameter.__method__ = HWWriter.UseSaved
                            collectionModel.append(writeParameter)
                            writer.writeParameterChanged(HWWriter.UseSaved, index)
                        })
            writer.userSavedWriteParameterRemoved.connect(
                        (index) => {
                            buttonGroup.checkState = Qt.Unchecked
                            collectionModel.remove(index)
                            writer.writeParameterChanged(HWWriter.Custom, -1)
                        })
        }*/
    }
}
