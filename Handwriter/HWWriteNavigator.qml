import QtQuick
import QtQuick.Layouts

import BGStudio.Controls
import BGStudio.Handwriter

QtObject {
    id: writeNavigator
    readonly property HWViewport viewport: HWViewport.viewport
    readonly property HWPaper paper: viewport.paper
    readonly property HWWriter writer: viewport.paper ? viewport.paper.canvas
                                                      : null

    property bool writeEndTimeOut: false
    property bool autoScroll: true

    property Timer writingTimer: Timer {
        interval: HWSettings.autoScrollInterval
        running: false
        onTriggered: {
            writeEndTimeOut = true
            if (autoScroll && viewport.zoomed)
                nextEmptySpace()
        }
        onRunningChanged: if (running) writeEndTimeOut = false
    }

    readonly property real leftMargin: {
        if (paper) {
            if (paper.guideLine.type === HWGuideLine.Grid) {
                var gridWidth = paper.guideLine.spacingPx * paper.scale
                var gridCount = Math.floor(viewport.width / gridWidth)

                return (viewport.width - gridWidth * gridCount) / 2
            } else
                paper.guideLine.spacingPx / 2 * paper.scale
        } else
            0
    }
    readonly property real topMargin: {
        if (paper) {
            if (paper.guideLine.type === HWGuideLine.Grid) {
                const gridHeight = paper.guideLine.spacingPx * paper.scale
                const gridCount = Math.max(Math.floor(viewport.height / gridHeight) - 1,
                                           1)

                return Math.max((viewport.height - gridHeight * gridCount) / 2,
                                0)
            } else {
                const scale = paper.scale
                const spacingPx = paper.guideLine.spacingPx * scale
                const lineHeightPx = paper.guideLine.lineHeightPx * scale
                const gridHeight = lineHeightPx + spacingPx
                const viewportHeight = viewport.height + spacingPx
                const gridCount = Math.max(Math.floor(viewportHeight / gridHeight) - 1,
                                           1)

                return Math.max((viewportHeight - gridHeight * gridCount) / 2,
                                0)
            }
        } else
            0
    }

    property var writingRect: []
    property var writingGridRect: []

    /*readonly property QtObject writingIndicatorRect: QtObject {
        property real x: {
            (writingGridRect.length > 0 ? writingGridRect[0] : 0)
                    * paper.scale + paper.x
        }
        property real y: {
            (writingGridRect.length > 0 ? writingGridRect[1] : 0)
                    * paper.scale + paper.y
        }
        property real width: {
            (writingGridRect.length > 0 ? writingGridRect[2] - writingGridRect[0] : 0)
                    * paper.scale
        }
        property real height: {
            (writingGridRect.length > 0 ? writingGridRect[3] - writingGridRect[1] : 0)
                    * paper.scale
        }
        readonly property bool visible: viewport.zoomed
                                        && writingRect.length > 0
                                        && writingGridRect.length > 0
    }*/

    /*property Item writingIndicator: Canvas {
        x: writeNavigator.writingIndicatorRect.x - 10
        y: writeNavigator.writingIndicatorRect.y - 10
        width: writeNavigator.writingIndicatorRect.width + 20
        height: writeNavigator.writingIndicatorRect.height + 20

        visible: writeNavigator.writingIndicatorRect.visible

        onPaint: {
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.lineWidth = 5
            ctx.strokeStyle =  ctx.createPattern(viewport.BGControls.accent,
                                                 Qt.BDiagPattern)
            ctx.beginPath()
            ctx.roundedRect(2.5, 2.5, width - 5, height - 5, 5, 5)
            ctx.stroke()
        }
    }*/
    property Item writingIndicator: null

    property int navButtonOrientation: Qt.Vertical
    readonly property bool navBtnVisible: viewport.zoomed && !writer.writing && writeEndTimeOut

    property list<Action> extToolBar0Actions: []
    property list<Action> extToolBar1Actions: []
    /*property Instantiator extToolBar0Buttons: Instantiator {
        asynchronous: true
        model: extToolBar0Actions
        Button {
            action: modelData
            visible: navButtonOrientation === Qt.Vertical
                     && (action.visibleWhenZoomed ?
                             viewport.zoomed : true)
        }
    }
    property Instantiator extToolBar1Buttons: Instantiator {
        asynchronous: true
        model: extToolBar1Actions
        Button {
            action: modelData
            visible: navButtonOrientation === Qt.Vertical
                     && (action.visibleWhenZoomed ?
                             viewport.zoomed : true)
            Component.onCompleted: console.log("index", index, modelData)
        }
    }*/

    property ColumnLayout vToolBar: ColumnLayout {
        parent: viewport
        visible: navButtonOrientation === Qt.Vertical

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 6

        z: 2

        ColumnLayout {
            id: clExtToolBar0
            Repeater {
                model: extToolBar0Actions
                Button {
                    action: modelData
                    visible: modelData.visibleWhenZoomed
                             ? viewport.zoomed : true
                }
            }
        }

        /*ToolSeparator {
            visible: navButtonOrientation === Qt.Vertical && clExtToolBar0.implicitHeight > 0
            orientation: Qt.Horizontal
            Layout.fillWidth: true
        }*/
        LayoutItemProxy {
            target: space0
        }

        LayoutItemProxy {
            visible: navButtonOrientation === Qt.Vertical && navBtnVisible
            target: btnNextEmptySpace
        }
        LayoutItemProxy {
            visible: navButtonOrientation === Qt.Vertical && navBtnVisible
            target: btnNextEmptyLine
        }
        LayoutItemProxy {
            target: btnAutoScroll
        }

        /*ToolSeparator {
            visible: navButtonOrientation === Qt.Vertical && clExtToolBar1.implicitHeight > 0
            orientation: Qt.Horizontal
            Layout.fillWidth: true
        }*/
        LayoutItemProxy {
            target: space1
        }

        ColumnLayout {
            id: clExtToolBar1
            Repeater {
                model: extToolBar1Actions
                Button {
                    action: modelData
                    visible: modelData.visibleWhenZoomed
                             ? viewport.zoomed : true
                }
            }
        }
    }

    property RowLayout hToolBar: RowLayout {
        parent: viewport
        visible: navButtonOrientation === Qt.Horizontal

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6

        z: 2

        RowLayout {
            id: rlExtToolBar0
            Repeater {
                model: extToolBar0Actions
                Button {
                    action: modelData
                    visible: modelData.visibleWhenZoomed
                             ? viewport.zoomed : true
                }
            }
        }

        /*ToolSeparator {
            visible: navButtonOrientation === Qt.Horizontal && rlExtToolBar0.implicitWidth > 0
            orientation: Qt.Vertical
            Layout.fillHeight: true
        }*/
        LayoutItemProxy {
            target: space0
        }

        LayoutItemProxy {
            visible: navButtonOrientation === Qt.Horizontal && navBtnVisible
            target: btnNextEmptySpace
        }
        LayoutItemProxy {
            visible: navButtonOrientation === Qt.Horizontal && navBtnVisible
            target: btnNextEmptyLine
        }
        LayoutItemProxy {
            target: btnAutoScroll
        }

        /*ToolSeparator {
            visible: navButtonOrientation === Qt.Horizontal && rlExtToolBar1.implicitWidth > 0
            orientation: Qt.Vertical
            Layout.fillHeight: true
        }*/
        LayoutItemProxy {
            target: space1
        }

        RowLayout {
            id: rlExtToolBar1
            Repeater {
                model: extToolBar1Actions
                Button {
                    action: modelData
                    visible: modelData.visibleWhenZoomed
                             ? viewport.zoomed : true
                }
            }
        }
    }

    property Item space0: Item {
        implicitWidth: 4
        implicitHeight: 4
    }
    property Item space1: Item {
        implicitWidth: 4
        implicitHeight: 4
    }

    property Button btnNextEmptySpace: Button {
        icon.source: "qrc:/qt/qml/BGStudio/Handwriter/icons/NextEmptySpace.png"

        onClicked: nextEmptySpace()
    }
    property Button btnNextEmptyLine: Button {
        icon.source: "qrc:/qt/qml/BGStudio/Handwriter/icons/NextEmptyLine.png"

        onClicked: nextEmptyLine()
    }
    property Button btnAutoScroll: Button {
        icon.source: "qrc:/qt/qml/BGStudio/Handwriter/icons/autoScroll.png"

        checked: autoScroll

        onClicked: autoScroll = !autoScroll
    }

    property real previewPanelWidth: viewport.width
    property real previewPanelHeight: 64

    property HWNavigatorPreview preview

    function nextEmptySpace() {
        viewport.panToX(writingGridRect[2], leftMargin)
    }

    function nextEmptyLine() {
        viewport.panToX(paper.guideLine.horizontalPaddingPx, leftMargin)
        viewport.panToY(writingGridRect[3] + paper.guideLine.spacingPx,
                        topMargin)
    }

    function updateWritingRect(pos) {
        if (writingRect.length === 0)
            writingRect = [pos.x, pos.y, pos.x, pos.y]
        else {
            writingRect[0] = Math.min(writingRect[0], pos.x)
            writingRect[1] = Math.min(writingRect[1], pos.y)
            writingRect[2] = Math.max(writingRect[2], pos.x)
            writingRect[3] = Math.max(writingRect[3], pos.y)
        }

        const horPadding = paper.guideLine.horizontalPaddingPx
        const topPadding = paper.guideLine.topPaddingPx
        const spacing = paper.guideLine.spacingPx

        if (paper.guideLine.type === HWGuideLine.Grid) {
            const leftCount = Math.floor((writingRect[0] - horPadding) / spacing)
            const left = horPadding + spacing * leftCount

            if (writingRect[0] > left + spacing * 0.75
                    && writingRect[2] > left + spacing
                    && writingRect[2] - left > spacing * 1.25)
                writingGridRect[0] = left + spacing
            else
                writingGridRect[0] = left

            const topCount = Math.floor((writingRect[1] - topPadding) / spacing)
            const top = topPadding + spacing * topCount

            if (writingRect[1] > top + spacing * 0.75
                    && writingRect[3] > top + spacing
                    && writingRect[3] - top > spacing * 1.25)
                writingGridRect[1] = top + spacing
            else
                writingGridRect[1] = top

            const gridCount = Math.max(Math.round((writingRect[2] - writingRect[0]) / spacing), 1)
            const rectWidth = gridCount * spacing
            const right = writingGridRect[0] + rectWidth
            if (writingRect[2] - right > spacing * 0.15)
                writingGridRect[2] = right + spacing
            else
                writingGridRect[2] = right

            writingGridRect[3] = writingGridRect[1] + spacing
        } else {
            const lineHeight = paper.guideLine.lineHeightPx + spacing

            const topCount = Math.floor((writingRect[1] - topPadding + spacing) /
                                        lineHeight)
            writingGridRect[0] = writingRect[0]
            writingGridRect[1] = topPadding + lineHeight * topCount
            writingGridRect[2] = writingRect[2]
            writingGridRect[3] = writingGridRect[1] + paper.guideLine.lineHeightPx
        }

        writingRectChanged()
        writingGridRectChanged()
    }

    property Connections connections: Connections {
        target: viewport.paper ? viewport.paper.canvas : null
        function onWriteStart(pos) {
            if (!writingTimer.running)
                writingRect = []
            else
                writingTimer.stop()

            updateWritingRect(pos)
        }
        function onWriteUpdated(stroke) {
            updateWritingRect(stroke.pos)
        }
        function onWriteEnd(stroke) {
            writingTimer.restart()
        }
        function onWriteIgnore() {
            writingTimer.stop()
        }
    }

    onWriterChanged: {
        writeEndTimeOut = false
    }

    Component.onCompleted: {
        if (writingIndicator)
            writingIndicator.parent = viewport

        if (preview) {
            preview.parent = viewport
            preview.viewport = viewport
            preview.writeNavigator = writeNavigator
            // preview.scrollEnabled = false
            // preview.snapshot.live = false
            writer.writingChanged.connect(
                        () => {
                            if (!writer.writing)
                                preview.grabSnapshot()
                        })
        }

        /*viewport.paper.canvas.writeStart.connect(
                    (pos) => {
                        if (!writingTimer.running)
                            writingRect = []
                        else
                            writingTimer.stop()

                        updateWritingRect(pos)
                    })
        viewport.paper.canvas.writeUpdated.connect(
                    (stroke) => {
                        updateWritingRect(stroke.pos)
                    })
        viewport.paper.canvas.writeEnd.connect(
                    (stroke) => {
                        writingTimer.restart()
                    })
        viewport.paper.canvas.writeIgnore.connect(
                    () => {
                        writingTimer.stop()
                    })*/
    }
}
