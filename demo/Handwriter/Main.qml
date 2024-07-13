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

    // property var pages: [1, 2]
    property HWPaper testPaper1: HWPaper {
        canvas: HWWriter {}
        paperSizeMm: "60x110"
        background: "qrc:/qt/qml/BGStudio/Handwriter/imgs/fabric_line.png"
        guideLine.layout: {
            "type": 1,
            "spacingMm": 2,
            "lineHeightMm": 4,
            "horizontalPadding": 4,
            "topPadding": 3,
            "bottomPadding": 4,
            "color": "#999999"
        }
        guideLine.visible: viewport.zoomed
    }
    property HWPaper testPaper2: HWPaper {
        canvas: HWWriter {}
        paperSizeMm: "60x110"
        background: "qrc:/qt/qml/BGStudio/Handwriter/imgs/fabric.png"
        guideLine.layout: {
            "type": 1,
            "spacingMm": 2,
            "lineHeightMm": 4,
            "horizontalPadding": 4,
            "topPadding": 3,
            "bottomPadding": 4,
            "color": "#999999"
        }
        guideLine.visible: viewport.zoomed
    }

    HWViewport {
        id: viewport
        anchors.fill: parent

        maxZoomRatio: 10
        zoomFactor: 1

        paper: testPaper1

        /*property alias paperX: paper.x
        property alias paperY: paper.y*/

        /*paper: HWPaper {
            canvas: HWWriter {}
            paperSizeMm: "60x110"
            background: "qrc:/qt/qml/BGStudio/Handwriter/imgs/fabric_line.png"
            guideLine.layout: {
                "type": 1,
                "spacingMm": 2,
                "lineHeightMm": 4,
                "horizontalPadding": 4,
                "topPadding": 3,
                "bottomPadding": 4,
                "color": "#999999"
            }
            guideLine.visible: viewport.zoomed
        }*/

        // singlePointEnable: true

        HWWriteNavigator {
            id: writeNavigator

            //autoScroll: false

            // preview: HWNavigatorPreview{}

            navButtonOrientation: viewport.width > viewport.height ? Qt.Vertical : Qt.Horizontal

            extToolBar0Actions: NavigatorToolbars.extToolBar0
            extToolBar1Actions: NavigatorToolbars.extToolBar1
        }

        QuickToolKit {
            id: bottomDrawer

            zoomLevel: [1, 3.3, 5, 6.6, 10]
        }
    }

    Drawer {
        id: rightDrawer
        edge: Qt.RightEdge

        dim: false

        width: Math.min(ApplicationWindow.window.width,
                        Math.max(300, ApplicationWindow.window.width * 0.33))
        height: ApplicationWindow.window.height

        interactive: !strokeKit.strokePreview.writer.writing
                     //&& !strokeKit.strokePreview.isGestureInProgress

        StrokeKit {
            id: strokeKit
            anchors.fill: parent
            writer: viewport.paper ? viewport.paper.canvas : null
        }
    }

    Drawer {
        id: previewDrawer

        edge: Qt.RightEdge

        width: window.width * 0.8
        height: width * viewport.height / viewport.width
        y: (window.height - height) / 2

        leftPadding: 0

        dim: false
        // modal: false

        interactive: !preview.isGestureInProgress

        HWViewportPreview {
            id: preview
            anchors.fill: parent
            viewport: viewport
            snapshot.outline: true
        }

        background: null

        onPositionChanged: {
            if (position === 1)
                preview.snapshot.grab()
        }
    }

    Connections {
        target: NavigatorToolbars
        function onClear() {
            viewport.paper.canvas.clear()
        }
        function onOpenStrokeAdjustmentDrawer() {
            rightDrawer.open()
        }
        function onOpenPreviewDrawer() {
            previewDrawer.open()
        }
        function onLoadFile() {
            viewport.paper.loadLocalStorageData(1)
        }
        function onSaveFile() {
            viewport.paper.saveLocalStorageData()
        }
    }

    // Component.onCompleted: console.log("------", HWDataConversion.b2a("blue ghost"))
}
