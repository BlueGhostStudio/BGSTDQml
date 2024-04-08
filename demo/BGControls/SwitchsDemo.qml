import QtQuick

import BGStudio.Controls

FieldItem {
    property ButtonGroup buttonGroup: ButtonGroup {
        exclusive: true
    }

    property var globalProp:  [{
            enabled: true
        }, {
            enabled: true
        }, {
            enabled: false
        }, {
            enabled: false
        }]
    FieldsGroupBox {
        title: "Switch Controls"
        DemoCtrlGroup {
            ctrlCmp: CheckBox { text: "CheckBox" }
            model: globalProp
        }
        DemoCtrlGroup {
            ctrlCmp: RadioButton {
                text: "RadioButton"
                ButtonGroup.group: buttonGroup
            }
            model: globalProp
        }
        DemoCtrlGroup {
            ctrlCmp: Switch { text: "Switch" }
            model: globalProp
        }
    }
}
