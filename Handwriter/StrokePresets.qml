import QtQuick
import "qrc:/qt/qml/BGStudio/Handwriter/Private" as P

P.StrokeCollection {
    Component.onCompleted: {
        const collection = HWSettings.penPresets
        for (let x in collection) {
            const strokeOptions = collection[x]
            collectionModel.append(
                        {
                                  "strokeColor": strokeOptions.strokeColor || 0,
                                   "strokeType": strokeOptions.strokeType || 0,
                                   "minPenSize": strokeOptions.minPenSize || 0.1,
                                   "strokeSize": strokeOptions.strokeSize || 1.15,
                                 "fadeoutLimit": strokeOptions.fadeoutLimit || 0.8,
                                  "fadeinLimit": strokeOptions.fadeinLimit || 1.2,
                            "velocityThreshold": strokeOptions.velocityThreshold || 400
                        })
        }

        buttonGroup.buttons[0].checked = true
    }
}
