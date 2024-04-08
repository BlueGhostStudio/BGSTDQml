import QtQuick
import QtQuick.Layouts
import BGStudio.Controls

Flow {
    id: control

    spacing: 0

    property list<var> options: []
    property int type: OptionField.Radio

    signal valueChanged(var value)

    FieldItem.orientation: Qt.Vertical

    enum Type {
        Radio,
        Check
    }

    Component {
        id: radioCmp
        RadioButton {
            property var value
            ButtonGroup.group: buttonGroup
        }
    }

    Component {
        id: checkCmp
        CheckBox {
            property var value
            ButtonGroup.group: buttonGroup
        }
    }

    ButtonGroup {
        id: buttonGroup
        exclusive: control.type === OptionField.Radio
    }

    function fieldValue() {
        if (control.type === OptionField.Radio) {
            var btnChecked = buttonGroup.checkedButton
            if (btnChecked)
                return btnChecked.value
            else
                return FieldItem.field.initialValue
        } else {
            var value = []
            for (var x in buttonGroup.buttons) {
                var button = buttonGroup.buttons[x]
                if (button.checked)
                    value.push(button.value)
            }

            return value
        }
    }

    Component.onCompleted: {
        var cmp = type === OptionField.Radio ? radioCmp : checkCmp

        var initialValue = FieldItem.field.initialValue;

        for (var x in options) {
            var button = cmp.createObject(control, {
                                              "text": options[x].label,
                                              "value": options[x].value
                                          })
            button.toggled.connect(() => {
                                       valueChanged(fieldValue())
                                   })
            if (initialValue !== undefined) {
                if (type === OptionField.Radio)
                    button.checked = button.value === initialValue
                else if (Array.isArray(initialValue)){
                    button.checked = initialValue.includes(button.value)
                }
            }
        }
    }
}
