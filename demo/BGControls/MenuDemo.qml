import QtQuick

import BGStudio.Controls

//import QtQuick.Controls
FieldItem {
    Button {
        text: "Menu"

        Menu {
            id: menuDemo
            MenuItem { text: "Cut" }
            MenuItem { text: "Copy" }
            MenuItem { text: "Paste" }
            Menu {
                title: "Indent"
                MenuItem {
                    text: "Space"
                    checkable: true
                }
                MenuItem {
                    text: "Indent select"
                }
            }
        }

        onClicked: {
            menuDemo.popup()
        }
    }
}
