import QtQuick

import BGStudio.Controls
import QtQuick.Layouts

//import QtQuick.Controls
DemoCtrlGroup {
    columns: 2
    ctrlCmp: GroupBox {
        id: groupBox
        property bool levelCounter: true
        property int border: GroupBox.Border
        property bool noTitleBackground: false
        property bool transparent: false

        BGControls.options: {
            "levelCounter": levelCounter,
            "border": border,
            "noTitleBackground": noTitleBackground,
            "transparent": transparent,
            "role": 2
        }

        title: "Top Level"

        width: 250

        GroupBox {
            title: "Sub GroupBox 0"

            BGControls.options: {
                "levelCounter": levelCounter,
                "border": border !== GroupBox.OnlyTopLevel ? border : GroupBox.Border,
                "noTitleBackground": noTitleBackground,
                "transparent": transparent
            }

            anchors.fill: parent

            FieldsGroupBox {
                anchors.fill: parent

                title: "GroupBox options"

                FieldItem {
                    initialValue: [0, 1]
                    OptionField {
                        type: OptionField.Check
                        options: [{
                                label: "levelCounter",
                                value: 0
                            }, {
                                label: "border",
                                value: 1
                            }, {
                                label: "noTitleBackground",
                                value: 2
                            }, {
                                label: "transparent",
                                value: 3
                            }
                        ]

                        onValueChanged: {
                            var value = fieldValue()
                            groupBox.levelCounter = value.includes(0)
                            groupBox.border = value.includes(1)
                            groupBox.noTitleBackground = value.includes(2)
                            groupBox.transparent = value.includes(3)
                        }
                    }
                }
            }
        }
    }

    model: [{
            "enabled": true
        }, {
            "enabled": false
        }
    ]
}
