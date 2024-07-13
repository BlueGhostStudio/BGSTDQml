pragma Singleton

import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls

QtObject {
    signal clear()
    signal openStrokeAdjustmentDrawer()
    signal openPreviewDrawer()
    signal saveFile()
    signal loadFile()

    property list<Action> extToolBar0: [Action {
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/menu.png"

            property Menu menu: Menu {
                MenuItem {
                    text: "Clear"
                    onTriggered: clear()//viewport.canvas.clear()
                }
                MenuItem {
                    text: "Load"
                    onTriggered: loadFile()
                }
                MenuItem {
                    text: "Save"
                    onTriggered: saveFile()
                }
            }

            onTriggered: function (source) {
                menu.parent = source
                menu.open()
            }
        }, Action {
            icon.source: "qrc:/qt/qml/BGStudio/Handwriter/icons/quill_pen.png"
            onTriggered: openStrokeAdjustmentDrawer()//rightDrawer.open()
        }]
    property list<Action> extToolBar1: [Action {
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/view.png"
            property bool visibleWhenZoomed: true

            onTriggered: openPreviewDrawer()//previewDrawer.open()
        }]
}
