import QtQuick
import BGStudio.Handwriter

import "qrc:/qt/qml/BGStudio/Handwriter/Private" as P

P.StrokeCollection {
    method: HWWriter.Preset

    onWriterChanged: {
        if (writer && writer.writeParameterMethod === HWWriter.Preset) {
            var index = writer.writeParameterIndex
            if (index >= 0 && index < buttonGroup.buttons.length)
                buttonGroup.buttons[index].checked = true
        } else
            buttonGroup.checkState = Qt.Unchecked
    }

    Component.onCompleted: {
        const collection = HWSettings.penPresets
        for (let x in collection) {
            const strokeOptions = collection[x]
            collectionModel.append(
                        {
                                   "__method__": HWWriter.Preset,
                                    "__index__": x,
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
