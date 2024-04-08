import QtQuick
import QtQuick.Effects
import QtQuick.Controls

import BGStudio.Controls
import QtQuick.Layouts

//import QtQuick.Controls
ApplicationWindow {
    id: window

    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    BGControls.covered: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action { text: qsTr("&New...") }
            Action { text: qsTr("&Open...") }
            Action { text: qsTr("&Save") }
            Action { text: qsTr("Save &As...") }
            MenuSeparator { }
            Action { text: qsTr("&Quit") }
        }
        Menu {
            title: qsTr("&Edit")
            Action { text: qsTr("Cu&t") }
            Action { text: qsTr("&Copy") }
            Action { text: qsTr("&Paste") }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
        }
    }

    header: ToolBar {
        ToolButton {
            text: "Menu"
            icon.source: "/qt/qml/BGStudio/Controls/icons/menu.png"
        }

        Text {
            BGText.textScale: BGText.Button
            text: "Controls Demo"
        }

        Item { Layout.fillWidth: true }

        ToolButton {
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/chevron_right.png"
            onClicked: leftDrawer.open()
        }

        ToolButton {
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/expand_more.png"
            onClicked: topDrawer.open()
        }

        ToolButton {
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/expand_less.png"
            onClicked: bottomDrawer.open()
        }

        ToolButton {
            icon.source: "qrc:/qt/qml/BGStudio/Controls/icons/chevron_left.png"
            onClicked: rightDrawer.open()
        }
    }

    TabBar {
        id: tabBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        //position: TabBar.Footer

        TabButton {
            text: "Preferences"
        }
        TabButton {
            text: "Controls Demo"
        }
        TabButton {
            text: "Containers Demo"
        }
        TabButton {
            text: "Popups Demo"
        }
        TabButton {
            text: "Palette Demo"
        }
    }

    Drawer {
        id: leftDrawer
        width: window.width * 0.33
        height: window.height

        edge: Qt.LeftEdge

        Text {
            anchors.centerIn: parent
            text: "leftDrawer"
        }
    }

    Drawer {
        id: rightDrawer
        width: window.width * 0.33
        height: window.height

        edge: Qt.RightEdge

        Text {
            anchors.centerIn: parent
            text: "rightDrawer"
        }
    }

    Drawer {
        id: topDrawer
        width: window.width
        height: window.height * 0.33

        edge: Qt.TopEdge

        Text {
            anchors.centerIn: parent
            text: "TopDrawer"
        }
    }

    Drawer {
        id: bottomDrawer
        width: window.width
        height: window.height * 0.33

        edge: Qt.BottomEdge

        Text {
            anchors.centerIn: parent
            text: "BottomDrawer"
        }
    }

    StackLayout {
        id: stackLayout
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom

        currentIndex: tabBar.currentIndex

        Customization { mainWin: window }
        FieldsGroupBox {
            title: "Controls Demo"
            ButtonsDemo {}
            SwitchsDemo {}
            InputsDemo {}
            SlidersDemo {}
        }
        FieldsGroupBox {
            title: "Containers Demo"
            //BGControls.covered: true
            PanesDemo {}
            GroupBoxsDemo {}
            FramesDemo {}
        }
        FieldsGroupBox {
            title: "Popups Demo"

            MenuDemo {}
            DialogDemo {}
        }
        PaletteDemo {
            title: "Palette Demo"
        }
    }
}
