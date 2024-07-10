import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    property string exportFileUrl

    Drawer {
        id: rightDrawer
        edge: Qt.RightEdge

        width: Math.min(ApplicationWindow.window.width,
                        Math.max(300, ApplicationWindow.window.width * 0.33))
        height: ApplicationWindow.window.height

        interactive: !strokeKit.strokePreview.writer.writing
                     && !strokeKit.strokePreview.gestureArea.isGestureInProgress

        StrokeKit {
            id: strokeKit
            anchors.fill: parent
            writer: viewport.canvas
        }
    }

    HWViewport {
        id: viewport
        anchors.fill: parent

        maxZoomRatio: 10
        zoomFactor: 1

        paper.paperSizeMm: "60x110"
        paper.background: Image {
            // fillMode: Image.Tile
            source: "qrc:/qt/qml/BGStudio/Handwriter/imgs/fabric_line.png"
        }

        canvas: HWWriter {
            // strokeColor: topDrawer.strokeColor
            // darkPalette: window.BGControls.foreground.hslLightness >= 0.5
            // guideLine.visible: viewport.paper.scale > 1
            activeNodes: viewport.zoomed ? HWCanvas.VisibleActiveWriting : HWCanvas.VisibleAllStroke
            // guideLine.visible: true
            guideLine.guideLine: {
                "type": 1,
                "spacingMm": 2,
                "lineHeightMm": 4,
                "horizontalPadding": 4,
                "topPadding": 9,
                "bottomPadding": 4,
                "color": "#999999"
            }
        }

        HWWriteNavigator {
            id: writeNavigator

            //autoScroll: false

            /*preview: HWViewportPreview{
                width: viewport.width - 128
                height: 64
            }*/

            navButtonOrientation: viewport.width > viewport.height ? Qt.Vertical : Qt.Horizontal

            extToolBar0: NavigatorToolbars.extToolBar0
            extToolBar1: NavigatorToolbars.extToolBar1
        }
    }

    /*Drawer {
        id: previewDrawer

        edge: Qt.RightEdge

        width: window.width * 0.5
        height: width * viewport.height / viewport.width
        y: (window.height - height) / 2

        leftPadding: 0

        dim: false
        modal: false

        interactive: !preview.isGestureInProgress

        HWViewportPreview {
            id: preview
            anchors.fill: parent
            viewport: viewport
        }

        background: null
    }*/

    Connections {
        target: NavigatorToolbars
        function onClear() {
            viewport.canvas.clear()
            /*viewport.canvas.activeNodes = viewport.canvas.activeNodes == HWCanvas.VisibleAllStroke
                    ? HWCanvas.VisibleActiveWriting : HWCanvas.VisibleAllStroke*/
        }
        function onOpenStrokeAdjustmentDrawer() {
            rightDrawer.open()
        }
        /*function onOpenPreviewDrawer() {
            previewDrawer.open()
        }*/
        function onExportToImage() {
            if (exportFileUrl.length ===  0)
                exportFileUrl = StandardPaths.writableLocation(StandardPaths.PicturesLocation)
                        + "/hw_" + Date.now() + ".png"
            viewport.paper.export2Image(exportFileUrl)
        }
    }

    Component.onCompleted: {
        /*HWSettings.userSavedStrokes = [{
                                         "strokeColor":2,
                                         "strokeType": 1,
                                         "minPenSize": 0.2,
                                         "strokeSize": 2,
                                         "fadeoutLimit": 0.8,
                                         "fadeinLimit": 1.2,
                                         "velocityThreshold": 100
                                     }, {
                                           "strokeColor":3,
                                           "strokeType": 1,
                                           "minPenSize": 0.1,
                                           "strokeSize": 1.5,
                                           "fadeoutLimit": 0.75,
                                           "fadeinLimit": 1.25,
                                           "velocityThreshold": 100
                                       }]*/
    }
}
