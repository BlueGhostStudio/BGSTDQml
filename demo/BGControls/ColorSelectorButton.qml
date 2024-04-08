import QtQuick
import QtQuick.Layouts

import BGStudio.Controls

//import QtQuick.Controls
Button {
    id: control
    property alias color: colorMixer.color

    padding: 4
    leftPadding: 4
    rightPadding: 4

    signal colorMixed()

    Dialog {
        id: dlgColorMix
        title: "Color Mixer"
        anchors.centerIn: Overlay.overlay

        ColorMixer {
            id: colorMixer

            BGControls.options: {
                "border": false,
                "transparent": true
            }

            color: "red"

            onMixed: control.colorMixed()
        }
        standardButtons: Dialog.Close
    }

    contentItem: Rectangle {
        color: control.color
        implicitWidth: 22
        implicitHeight: 22
        radius: 2
    }

    onClicked: dlgColorMix.open()
}
