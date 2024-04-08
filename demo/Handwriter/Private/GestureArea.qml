import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls

Item {
    id: gestureArea

    property HWPaper paper
    property Item viewport
    property bool isGestureInProgress: panHandler.active || zoomHandler.active

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
                zoom = Math.min(Math.max(lastZoom
                                         + (point.position.x
                                            - point.pressPosition.x) / 200,
                                         0), 1)
                viewport.zoom(zoom, point.pressPosition.x,
                                 point.pressPosition.y)
            }
        }
    }
}
