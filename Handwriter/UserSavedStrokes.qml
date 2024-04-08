import QtQuick
import BGStudio.Controls
import BGStudio.Handwriter

import "qrc:/qt/qml/BGStudio/Handwriter/Private" as P

P.StrokeCollection {
    signal requesSave()

    Button {
        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/add.png"

        onClicked: requesSave()
    }

    function saveStroke(strokeOptions) {
        collectionModel.append(strokeOptions)
        HWSettings.saveUserStroke(strokeOptions)
        var button = buttonGroup.buttons[collectionModel.count - 1]
        button.checked = true
        button.clicked()
    }

    function updateStroke(index, strokeOptions) {
        collectionModel.set(index, strokeOptions)
        HWSettings.updateUserStroke(index, strokeOptions)
    }

    function removeStroke(index) {
        collectionModel.remove(index)
        HWSettings.removeUserStroke(index)
    }

    Component.onCompleted: {
        const collection = HWSettings.userSavedStrokes
        for (let x in HWSettings.userSavedStrokes) {
            const strokeOptions = collection[x]
            collectionModel.append(
                        {
                                  "strokeColor": strokeOptions.strokeColor || 0,
                                   "strokeType": strokeOptions.strokeType || 0,
                                   "minPenSize": strokeOptions.minPenSize || 0.1,
                                   "strokeSize": strokeOptions.strokeSize || 1.15,
                                 "fadeoutLimit": strokeOptions.fadeoutLimit || 0.8,
                                  "fadeinLimit": strokeOptions.fadeinLimit || 1.2,
                            "velocityThreshold": strokeOptions.velocityThreshold || 95
                        })
        }
    }
}
