import QtQuick

import BGStudio.Controls
import BGStudio.Handwriter

Frame {
    id: preview
    property HWViewport viewport
    readonly property HWPaper paper: viewport.paper
    readonly property HWCanvas canvas: viewport.canvas
    property QtObject writeNavigator: null

    readonly property real paperX: paper ? -paper.x / viewport.paper.scale : 0
    readonly property real paperY: paper ? -paper.y / viewport.paper.scale : 0
    readonly property real viewportWidth: viewport ? viewport.width / paper.scale : 0
    readonly property real viewportHeight: viewport ? viewport.height / paper.scale : 0

    padding: 3

    readonly property real snapshotPaperX: {
        if (viewport && writeNavigator) {
            const rightEdge = (viewport.width - paper.x) / paper.scale
            if (rightEdge > snapshot.width)
                return rightEdge - snapshot.width;
            else
                return 0
        } else
            return 0
    }
    readonly property real snapshotPaperY: {
        if (viewport && writeNavigator) {
            if (writeNavigator.writingRect.length > 0
                    && writeNavigator.writingGridRect.length > 0) {
                return writeNavigator.writingGridRect[1]
                        - (snapshot.height
                           - writeNavigator.writingGridRect[3]
                           + writeNavigator.writingGridRect[1]) / 2
            } else
                return (snapshot.height - viewportHeight) / 2
                        - paper.y / paper.scale
        }
    }

    width: viewport ? Math.min(viewport.width, paper.width + 3) : 0;
    height: Math.min(viewportHeight + 6, 64)
    x: writeNavigator ? (viewport.width - width) / 2 : 0

    visible: false

    Behavior on y {
        enabled: writeNavigator
        NumberAnimation {}
    }

    contentItem: Rectangle {
        color: viewport.BGControls.background
        radius: 2
        ShaderEffectSource {
            id: snapshot

            width: parent.width
            height: parent.height

            live: false
            sourceItem: canvas
            sourceRect: Qt.rect(snapshotPaperX, snapshotPaperY, width, height)
        }
    }

    function grabSnapshot() {
        snapshotState = 1
        canvas.contentVisible = HWCanvas.VisibleLine
    }

    states: [
        State{
            name: ""
            PropertyChanges {
                target: preview
                visible: false
            }
        },
        State {
            name: "zoomed"
            when: viewport && writeNavigator && paper.scale > 5
            PropertyChanges {
                target: preview
                visible: true
            }
        }
    ]

    property int snapshotState: 0

    Component.onCompleted: {
        ApplicationWindow.windowChanged.connect(
                    () => {
                        if (ApplicationWindow && ApplicationWindow.window) {
                            ApplicationWindow.window.frameSwapped.connect(
                                () => {
                                    if (snapshotState === 1
                                        && canvas.contentVisible === HWCanvas.VisibleLine) {
                                        snapshot.scheduleUpdate();
                                        snapshotState = 2
                                    } else if (snapshotState === 2){
                                        snapshotState = 0
                                        canvas.contentVisible = HWCanvas.VisibleViewportOnly
                                    }
                                })
                        }
                    })
    }
}
