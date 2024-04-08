import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

ComboBox {
    id: strokeSelector
    textRole: "label"
    valueRole: "stroke"

    model: ListModel {}
    contentItem: HWStrokeSample {
        id: preview
        implicitHeight: 32
        readOnly: true

        darkPalette: strokeSelector.BGControls.foreground.hslLightness >= 0.5

        onWidthChanged: update()

        function update() {
            if (model.count > 0) {
                var stroke = strokeSelector.model.get(currentIndex).stroke
                strokeType = stroke.strokeType
                minPenSize = stroke.minPenSize
                strokeSize = stroke.strokeSize
                fadeoutLimit = stroke.fadeoutLimit
                fadeinLimit = stroke.fadeinLimit
                velocityThreshold = stroke.velocityThreshold
                clear()
                updateSineWavePreview(preview.width - 20, 16)
            }
        }
    }

    delegate: ItemDelegate {
        id: itemDelegate
        required property var model
        required property int index

        width: ListView.view.width
        height: 32

        highlighted: strokeSelector.highlightedIndex === index
        hoverEnabled: strokeSelector.hoverEnabled

        contentItem: HWStrokeSample {
            id: delegatePreview

            readOnly: true
            darkPalette: itemDelegate.BGButton.itemColor.hslLightness >= 0.5

            onDarkPaletteChanged: update()

            function update() {
                clear()
                var stroke = strokeSelector.model.get(index).stroke
                strokeType = stroke.strokeType
                minPenSize = stroke.minPenSize
                strokeSize = stroke.strokeSize
                fadeoutLimit = stroke.fadeoutLimit
                fadeinLimit = stroke.fadeinLimit
                velocityThreshold = stroke.velocityThreshold
                updateSineWavePreview(preview.width - 20, 16)
            }
        }

        Component.onCompleted: {
            BGControls.forceRefresh()
            delegatePreview.update()
        }
    }

    onCurrentIndexChanged: preview.update()

    Component.onCompleted: {
        for(let x in HWSettings.penPresets) {
            model.append({
                             "label": HWSettings.penPresets[x].label,
                             "stroke": {
                                 "strokeType": HWSettings.penPresets[x].strokeType || 0,
                                 "minPenSize": HWSettings.penPresets[x].minPenSize || 0.1,
                                 "strokeSize": HWSettings.penPresets[x].strokeSize || 1.15,
                                 "fadeoutLimit": HWSettings.penPresets[x].fadeoutLimit || 0.8,
                                 "fadeinLimit": HWSettings.penPresets[x].fadeinLimit || 1.2,
                                 "velocityThreshold": HWSettings.penPresets[x].velocityThreshold || 400
                             }
                         })
        }
        currentIndex = 0
        contentItem.update()
    }
}
