import QtQuick

import BGStudio.Controls
import BGStudio.Handwriter

Frame {
    id: preview
    property HWViewport viewport
    readonly property HWPaper paper: viewport.paper
    readonly property HWCanvas canvas: viewport.canvas
    property QtObject writeNavigator: null

    property bool syncToViewportEnabled: false
    property bool scrollEnabled: true

    readonly property bool isGestureInProgress: canvasSnapshotTapHandler.active
                                                || previewPointHandler.active

    readonly property real paperX: paper ? -paper.x / viewport.paper.scale : 0
    readonly property real paperY: paper ? -paper.y / viewport.paper.scale : 0
    readonly property real viewportWidth: viewport ? viewport.width / paper.scale : 0
    readonly property real viewportHeight: viewport ? viewport.height / paper.scale : 0

    property Item viewportIndicator: Canvas {
        id: cvsViewportIndicator
        x: paperX - 10//paper ? -paper.x / viewport.paper.scale - 10 : 0
        y: paperY - 10//paper ? -paper.y / viewport.paper.scale - 10 : 0
        width: viewportWidth + 20
        height: viewportHeight + 20

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            if (preview.syncToViewportEnabled) {
                ctx.lineWidth = 5
                ctx.setLineDash([])
                ctx.strokeStyle =  ctx.createPattern(viewport.BGControls.accent,
                                                     Qt.BDiagPattern)
                ctx.beginPath()
                ctx.roundedRect(2.5, 2.5, width - 5, height - 5, 5, 5)
                ctx.stroke()
            }

            ctx.lineWidth = 1
            ctx.setLineDash([2, 2])
            ctx.strokeStyle = viewport.BGControls.foreground
            ctx.strokeRect(10, 10, width - 20, height - 20)
        }

        //preview.onSyncToViewportEnabledChanged: requestUpdate()
        Connections {
            target: preview
            function onSyncToViewportEnabledChanged() {
                cvsViewportIndicator.requestPaint()
            }
        }
    }

    x: writeNavigator ? (viewport.width - width) / 2 : 0
    y: writeNavigator ? -height : 0

    Behavior on y {
        enabled: writeNavigator
        NumberAnimation {}
    }

    contentItem: Rectangle {
        clip: true
        color: viewport.BGControls.background

        ShaderEffectSource {
            id: canvasSnapshot
            // samples: 8

            sourceItem: canvas

            width: paper ? paper.width : 0
            height: paper ? paper.height : 0

            TapHandler {
                id: canvasSnapshotTapHandler
                enabled: scrollEnabled

                onTapped: {
                    syncToViewport(point.position.x, point.position.y)
                    syncFromViewport()
                }
                onLongPressed: {
                    syncToViewportEnabled = true
                    const pressX = point.position.x
                    const pressY = point.position.y
                    if (pressX < paperX
                            || pressX > paperX + viewportWidth
                            || pressY < paperY
                            || pressY > paperY + viewportHeight)
                        syncToViewport(pressX, pressY)
                }
            }
        }

        ToolButton {
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            visible: !preview.writeNavigator
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/target.png"

            onClicked: syncFromViewport()
        }
    }

    PointHandler {
        id: previewPointHandler
        property real lastX: 0
        property real lastY: 0

        enabled: scrollEnabled

        onActiveChanged:  {
            if (active) {
                lastX = point.position.x
                lastY = point.position.y
            } else if (syncToViewportEnabled) {
                viewport.panAniEnabled = true
                syncToViewportEnabled = false
                syncFromViewport()
            }
        }

        onPointChanged: {
            if (active) {
                const distX = point.position.x - lastX
                const distY = point.position.y - lastY
                if (!syncToViewportEnabled) {
                    canvasSnapshot.x += distX
                    canvasSnapshot.y += distY
                } else {
                    viewport.panAniEnabled = false
                    viewport.panTo(preview.paperX + distX,
                                   preview.paperY + distY)
                }

                lastX = point.position.x
                lastY = point.position.y
            }
        }
    }

    function syncToViewport (dx, dy) {
        viewport.panTo(dx - viewportWidth / 2,
                       dy - viewportHeight / 2)
    }

    function syncFromViewport() {
        canvasSnapshot.x = Qt.binding(function () {
            if (viewport) {
                if (writeNavigator) {
                    const rightEdge = (viewport.width - paper.x) / paper.scale
                    if (rightEdge > contentItem.width)
                        return contentItem.width - rightEdge
                    else
                        return 0
                } else
                    return paper.x /  paper.scale
                            + (contentItem.width - viewportWidth) / 2
            } else
                return 0
        });
        canvasSnapshot.y = Qt.binding(function () {
            if (viewport) {
                if (writeNavigator) {
                    if (writeNavigator.writingRect.length > 0
                            && writeNavigator.writingGridRect.length > 0)
                        return -writeNavigator.writingGridRect[1] +
                                (contentItem.height
                                 - writeNavigator.writingGridRect[3]
                                 + writeNavigator.writingGridRect[1])
                                / 2
                    else
                        return (paper.y - viewport.height / 2) / paper.scale
                                + contentItem.height / 2
                } else {
                    return paper.y / paper.scale
                            + (contentItem.height - viewportHeight) / 2
                }
            } else
                return 0
        });
    }

    states: [
        State{
            name: ""
            PropertyChanges {
                target: preview
                y: -height
            }
        },
        State {
            name: "zoomed"
            when: viewport && writeNavigator && viewport.zoomed
            PropertyChanges {
                target: preview
                y: 0
            }
        }
    ]
    Component.onCompleted: {
        if (viewportIndicator)
            viewportIndicator.parent = canvasSnapshot

        syncFromViewport()
    }
}
