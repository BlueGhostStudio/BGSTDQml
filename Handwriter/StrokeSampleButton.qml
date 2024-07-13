import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

Button {
    id: control
    property var writeParameter
    property int strokeColor: 1
    property alias velocityFactor: strokeSample.velocityFactor

    checkable: true

    padding: 6

    contentItem: Item {
        implicitWidth: 22
        implicitHeight: 22

        HWStrokeSample {
            id: strokeSample

            anchors.centerIn: parent
            implicitWidth: 22
            implicitHeight: 22

            readOnly: true
            darkPalette: control.BGButton.itemColor.hslLightness >= 0.5
            velocityFactor: 10

            onDarkPaletteChanged: refresh()

            function refresh() {
                if (control.writeParameter) {
                    writeParameter = control.writeParameter
                    updateFromSetting()
                }
                // updateCircularPreview(8)
            }

            Component.onCompleted: {
                loadFromSetting()
            }
        }
    }

    // onCheckedChanged: strokeSample.refresh()

    onWriteParameterChanged: {
        BGControls.forceRefresh()
        strokeSample.refresh()
    }
}
