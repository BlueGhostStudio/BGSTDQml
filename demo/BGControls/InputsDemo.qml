import QtQuick

import BGStudio.Controls

FieldItem {
    property var globalProp: [{
            "enabled": true
        }, {
            "enabled": false
        }]
    FieldsGroupBox {
        title: "Switch Controls"
        DemoCtrlGroup {
            ctrlCmp: TextField {
                placeholderText: "TextField"
                width: 150
            }
            model: globalProp
        }
        DemoCtrlGroup {
            ctrlCmp: TextArea {
                placeholderText: "TextArea"
                width: 150
                height: 100
            }
            model: globalProp
        }
        DemoCtrlGroup {
            ctrlCmp: ComboBox {
                model: ["item 1", "item 2", "item 3"]
                width: 150
            }
            model: [{
                    "enabled": true,
                    "editable": false
                }, {
                    "enabled": true,
                    "editable": true
                }, {
                    "enabled": false,
                    "editable": false
                }, {
                    "enabled": false,
                    "editable": true
                }]
        }
        DemoCtrlGroup {
            ctrlCmp: SpinBox {
                from: 0; to: 10
            }
            model: [{
                    "enabled": true,
                    "editable": false
                }, {
                    "enabled": true,
                    "editable": true
                }, {
                    "enabled": false,
                    "editable": false
                }, {
                    "enabled": false,
                    "editable": true
                }]
        }
    }
}
