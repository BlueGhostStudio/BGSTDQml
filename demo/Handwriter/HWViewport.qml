import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls

import "Private" as P

Item {
    id: viewport
    property alias paper: paper
    property HWCanvas canvas

    HWPaper {
        id: paper
        canvas: viewport.canvas

        transformOrigin: Item.TopLeft
        Behavior on x {
            enabled: !gestureArea.isGestureInProgress
            NumberAnimation { duration: 250 }
        }
        Behavior on y {
            enabled: !gestureArea.isGestureInProgress
            NumberAnimation { duration: 250 }
        }

        /*function pan(dx, dy) {
            clampPosition(x + dx, y + dy);
        }

        function zoom(zoom, pressX, pressY) {
            var newScale = 1 + zoom * 100
            var scaleFactor = newScale / scale
            scale = newScale
            x -= (pressX - x) * scaleFactor - (pressX - x)
            y -= (pressY - y) * scaleFactor - (pressY - y)

            clampPosition(x, y)
        }

        function clampPositionX(_x) {
            if (width < viewport.width)
                x = (viewport.width - width) / 2
            else
                x = Math.max(Math.min(_x, 0), viewport.width - width * scale)
        }
        function clampPositionY(_y) {
            if (height < viewport.height)
                y = (viewport.height - height) / 2
            else
                y = Math.max(Math.min(_y, 0), viewport.height - height * scale)
        }
        function clampPosition(_x, _y) {
            clampPositionX(_x)
            clampPositionY(_y)
        }*/
    }

    P.GestureArea {
        id: gestureArea
        //viewport: viewport
        paper: viewport.paper

        anchors.fill: parent
    }

    function pan(dx, dy) {
        clampPosition(paper.x + dx, paper.y + dy);
    }

    function panTo(dx, dy) {
        clampPosition(-dx * paper.scale, -dy * paper.scale)
    }

    function panToX(dx, margin) {
        clampPositionX(-dx * paper.scale + (margin || 0))
    }

    function panToY(dy, margin) {
        clampPositionY(-dy * paper.scale + (margin || 0))
    }

    function panLeft(dx, margin) {
        clampPositionX(paper.x - dx * paper.scale + (margin || 0))
    }

    function zoom(zoom, pressX, pressY) {
        var newScale = 1 + zoom * 100
        var scaleFactor = newScale / paper.scale
        paper.scale = newScale
        paper.x -= (pressX - paper.x) * scaleFactor - (pressX - paper.x)
        paper.y -= (pressY - paper.y) * scaleFactor - (pressY - paper.y)

        clampPosition(paper.x, paper.y)
    }

    function clampPositionX(_x) {
        var scaledWidth = paper.width * paper.scale
        if (scaledWidth < viewport.width)
            paper.x = (viewport.width - scaledWidth) / 2
        else
            paper.x = Math.max(Math.min(_x, 0), viewport.width - scaledWidth)
    }
    function clampPositionY(_y) {
        var scaledHeight = paper.height * paper.scale
        if (scaledHeight < viewport.height)
            paper.y = (viewport.height - scaledHeight) / 2
        else
            paper.y = Math.max(Math.min(_y, 0),
                               viewport.height - scaledHeight)
    }
    function clampPosition(_x, _y) {
        clampPositionX(_x)
        clampPositionY(_y)
    }

    onWidthChanged: clampPositionX(paper.x)
    onHeightChanged: clampPositionY(paper.y)
}
