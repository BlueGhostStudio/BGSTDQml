import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

/*HWStrokeSample {
    id: strokeSample

    property HWWriter writer

    clip: true

    ToolButton {
        id: tbReset
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/reset.png"

        onClicked: strokeSample.resetRecord()
        onPressAndHold: {
            strokeSample.resetRecord()
            strokeSample.updateSineWavePreview(strokeSample.width - 32, 32)
        }
    }
    // Text {
    //     anchors.right: tbReset.left
    //     anchors.bottom: tbReset.bottom

    //     BGText.textScale: BGText.Overline

    //     text: "Max velocity: " + writer.maxVelocity.toFixed(2) + "\n" +
    //           "Writer combinedScale: " + writer.combinedScale.toFixed(2) + "\n" +
    //           "Velocity threshold: " + writer.scaledVelocityThreshold.toFixed(2)
    // }

    onStrokeTypeChanged: replayRecord()
    onMinPenSizeChanged: replayRecord()
    onStrokeSizeChanged: replayRecord()
    onFadeoutLimitChanged: replayRecord()
    onFadeinLimitChanged: replayRecord()
    onVelocityThresholdChanged: replayRecord()

    Component.onCompleted: {
        record()
        updateSineWavePreview(width - 32, 32)
    }
}*/

HWViewport {
    paper.paperSizeMm: "17x17"
    paper.background: Rectangle {
        color: "white"
    }

    property alias writer: strokeSample

    canvas: HWStrokeSample {
        id: strokeSample
        //guideLine.preset: HWGuideLine.Grid
        guideLine.visible: true
        guideLine.guideLine: {
            "type": 0,
            "spacingMm": 4,
            "horizontalPadding": 0.5,
            "topPadding": 0.5,
            "bottomPadding": 0.5
        }

        onStrokeTypeChanged: replayRecord()
        onMinPenSizeChanged: replayRecord()
        onStrokeSizeChanged: replayRecord()
        onFadeoutLimitChanged: replayRecord()
        onFadeinLimitChanged: replayRecord()
        onVelocityThresholdChanged: replayRecord()

        Component.onCompleted: record()
    }

    ToolButton {
        id: tbReset
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/reset.png"

        onClicked: strokeSample.resetRecord()
    }
    Text {
        anchors.right: tbReset.left
        anchors.bottom: tbReset.bottom

        BGText.textScale: BGText.Overline

        text: "Max velocity: " + strokeSample.maxVelocity.toFixed(2) + "\n" +
              "Writer combinedScale: " + strokeSample.combinedScale.toFixed(2) + "\n" +
              "Velocity threshold: " + strokeSample.scaledVelocityThreshold.toFixed(2)
    }
    ShaderEffectSource {
        anchors.right: parent.right
        anchors.top: parent.top
        sourceItem: paper
        width: paper.width
        height: paper.height
    }

    gestureArea.enabled: false

    // Component.onCompleted: console.log(gestureArea.isG

    Component.onCompleted: {
        zoom(10, 0, 0)
        panTo(paper.width / 2 - width / 20, paper.height / 2 - height / 20)
        widthChanged.connect(() => {
                                 panToX(paper.width / 2 - width / 20)
                             })
        heightChanged.connect(() => {
                                 panToY(paper.height / 2 - height / 20)
                             })
    }
}
