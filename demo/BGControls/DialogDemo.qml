import QtQuick

import BGStudio.Controls
import QtQuick.Layouts

//import QtQuick.Controls
FieldItem {
    Button {
        text: "Open Dialog"

        Dialog {
            id: dialog
            title: "Login"
            standardButtons: Dialog.Ok | Dialog.Cancel

            anchors.centerIn: Overlay.overlay

            modal: true

            FieldsLayout {
                FieldItem {
                    label: "User"
                    TextField {
                        placeholderText: "User"
                    }
                }
                FieldItem {
                    label: "Password"
                    TextField {
                        placeholderText: "Password"
                    }
                }
            }

            onAccepted: console.log("Ok clicked")
            onRejected: console.log("Cancel clicked")
        }

        onClicked: dialog.open()
    }
}
