import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls

Item {
    id: gestureArea

    readonly property bool isGestureInProgress: panHandler.active || zoomHandler.active

    onIsGestureInProgressChanged: function() {
        viewport.panAniEnabled = !isGestureInProgress
    }

    PointHandler {
        id: panHandler

        property real lastX: 0
        property real lastY: 0

        acceptedDevices: PointerDevice.Mouse
        acceptedButtons: Qt.LeftButton
        acceptedModifiers: Qt.ControlModifier

        cursorShape: Qt.DragMoveCursor

        onActiveChanged: {
            if (active) {
                lastX = point.position.x
                lastY = point.position.y
            }
        }

        onPointChanged: {
            if (active) {
                viewport.pan(point.position.x - lastX, point.position.y - lastY)
                lastX = point.position.x
                lastY = point.position.y
            }
        }
    }

    PointHandler {
        id: zoomHandler

        property real zoom: 0
        property real lastZoom: 0

        acceptedDevices: PointerDevice.Mouse
        acceptedButtons: Qt.LeftButton
        acceptedModifiers: Qt.ShiftModifier

        cursorShape: Qt.SizeHorCursor

        onActiveChanged: {
            if (active) {
                lastZoom = zoom
            }
        }

        onPointChanged: {
            if (active) {
                const dist = point.position.x - point.pressPosition.x

                zoom = (dist >= 0 ? lastZoom : lastZoom + viewport.zoomFactor)
                              + Math.min(dist / 200, 1) * 100
                zoom = Math.floor(zoom / viewport.zoomFactor) * viewport.zoomFactor
                zoom = Math.min(Math.max(zoom, 1), viewport.maxZoomRatio)

                viewport.zoom(zoom, point.pressPosition.x,
                                 point.pressPosition.y)
            }
        }
    }
}
