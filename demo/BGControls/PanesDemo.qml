import QtQuick

import BGStudio.Controls
import QtQuick.Layouts

//import QtQuick.Controls
DemoCtrlGroup {
    columns: 2
    ctrlCmp: Pane {
        id: pane
        BGControls.options: {
            "transparent": chbTransparent.checked
        }

        width: 250

        contentItem: ColumnLayout {
            Text {
                text: "Pane"
            }
            CheckBox {
                id: chbTransparent
                text: "Transparent"
            }
        }
    }
    model: [{
            "enabled": true
        }, {
            "enabled": false
        }]
}
