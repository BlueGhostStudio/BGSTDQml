pragma Singleton

import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls

QtObject {
    signal clear()
    signal openStrokeAdjustmentDrawer()
    signal openPreviewDrawer()
    signal exportToImage()

    property list<Item> extToolBar0: [Button {
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/menu.png"

            Menu {
                id: mainMenu

                MenuItem {
                    text: "Clear"
                    onTriggered: clear()//viewport.canvas.clear()
                }
                MenuItem {
                    text: "Export to image"
                    onTriggered: exportToImage()
                }
            }

            onClicked: mainMenu.open()
        }, Button {
            icon.source: "qrc:/qt/qml/BGStudio/Handwriter/icons/quill_pen.png"
            onClicked: openStrokeAdjustmentDrawer()//rightDrawer.open()
        }]
    property list<Item> extToolBar1: [Button {
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/view.png"
            property bool visibleWhenZoomed: true

            onClicked: openPreviewDrawer()//previewDrawer.open()
        }]
}
