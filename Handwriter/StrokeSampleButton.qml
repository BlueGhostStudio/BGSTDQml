import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

Button {
    id: control
    property var options
    property int strokeColor: 1

    checkable: true

    padding: 6

    contentItem: HWStrokeSample {
        id: strokeSample
        implicitWidth: 22
        implicitHeight: 22

        readOnly: true
        darkPalette: control.BGButton.itemColor.hslLightness >= 0.5

        onDarkPaletteChanged: refresh()

        function refresh() {
            clear()
            strokeColor = options.strokeColor
            strokeType = options.strokeType
            minPenSize = options.minPenSize
            strokeSize = options.strokeSize
            fadeoutLimit = options.fadeoutLimit
            fadeinLimit = options.fadeinLimit
            velocityThreshold = options.velocityThreshold
            updateCircularPreview(8)
        }
    }

    onCheckedChanged: strokeSample.refresh()

    onOptionsChanged: {
        BGControls.forceRefresh()
        strokeSample.refresh()
    }
}
