import QtQuick
import QtQuick.Layouts

import BGStudio.Controls

FieldItem {
    property var globalProp:  [{}, {
            highlighted: true
        }, {
            checkable: true
        }, {
            enabled: false
        }]

    FieldsGroupBox {
        title: "Buttons"

        DemoCtrlGroup {
            columns: 4
            ctrlCmp: Button {
                text: "Button"; icon.source: "bgstudio"
            }
            model: globalProp
        }
        DemoCtrlGroup {
            columns: 4
            ctrlCmp: ToolButton { text: "toolButton"; icon.source: "bgstudio" }
            model: globalProp
        }
        DemoCtrlGroup {
            columns: 4
            ctrlCmp: RoundButton { icon.source: "bgstudio" }
            model: globalProp
        }
    }
}
