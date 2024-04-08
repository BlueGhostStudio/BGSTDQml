import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls

FieldsGroupBox {
    id: strokeSettings
    property HWWriter writer
    property alias strokeColor: strokeColorSelector.strokeColor
    property alias strokeType: cbType.currentIndex
    property alias minPenSize: rsSize.first.value
    property alias strokeSize: rsSize.second.value
    property alias fadeoutLimit: rsFade.first.value
    property alias fadeinLimit: rsFade.second.value
    property alias velocityThreshold: sliVelocityThreshold.value

    property var options: {
        "strokeColor":strokeColor,
        "strokeType": strokeType,
        "minPenSize": minPenSize,
        "strokeSize": Math.round(strokeSize * 100) / 100,
        "fadeoutLimit": Math.min(fadeoutLimit, 1),
        "fadeinLimit": Math.max(fadeinLimit, 1),
        "velocityThreshold": velocityThreshold
    }

    signal activated()

    title: "Stroke Settings"

    FieldItem {
        label: "Type"
        field: "type"

        ComboBox {
            id: cbType
            model: ["Ballpoint", "Pen"]
            implicitWidth: 50

            onActivated: strokeSettings.activated()
        }
    }

    FieldItem {
        label: "Color"

        StrokeColorSelectorComboBox {
            id: strokeColorSelector

            onActivated: strokeSettings.activated()
        }
    }

    FieldItem {
        //label: "Size" + control.value.toFixed(2) + "]"
        label: "Size[" + control.first.value.toFixed(2) + "|" +
               control.second.value.toFixed(2) + "]"
        field: "size"
        orientation: Qt.Vertical

        RangeSlider {
            id: rsSize
            BGControls.options: {
                "tick": true
            }

            from: 0
            to: 10
            implicitWidth: 50

            stepSize: 0.02
            snapMode: RangeSlider.SnapAlways

            first.value: 0.1
            second.value: 1.15

            first.onMoved:  strokeSettings.activated()
            second.onMoved: strokeSettings.activated()
        }
    }

    FieldItem {
        label: "Fade[out:"
               + Math.min(control.first.value, 1).toFixed(2)
               + "|in:"
               + Math.max(control.second.value, 1).toFixed(2)
               + "]"
        field: "fade"
        orientation: Qt.Vertical

        RangeSlider {
            id: rsFade
            BGControls.options: {
                "tick": true
            }

            from: 0
            to: 2

            stepSize: 0.02
            snapMode: RangeSlider.SnapAlways

            implicitWidth: 50

            first.value: 0.8
            second.value: 1.2

            first.onMoved:  strokeSettings.activated()
            second.onMoved: strokeSettings.activated()
        }
    }

    FieldItem {
        label: "Velocity Threshold[" + control.value + "]"
        field: "velocityThreshold"
        orientation: Qt.Vertical

        Slider {
            id: sliVelocityThreshold
            BGControls.options: {
                "tick": true
            }

            from: 0
            to: 1000

            stepSize: 1
            snapMode: RangeSlider.SnapAlways

            implicitWidth: 50

            value: 95

            onMoved: strokeSettings.activated()
        }
    }

    function loadSettings(options) {
        if (options) {
            strokeColorSelector.currentIndex = options.strokeColor
            cbType.currentIndex = options.strokeType
            rsSize.first.value = options.minPenSize
            rsSize.second.value = options.strokeSize
            rsFade.first.value = Math.min(options.fadeoutLimit, 1)
            rsFade.second.value = Math.max(options.fadeinLimit, 1)
            sliVelocityThreshold.value = options.velocityThreshold
        }
    }

    Component.onCompleted: {
        if (writer) {
            optionsChanged.connect(() => {
                                      writer.loadSettings(options)
                                  })
        }
    }
}
