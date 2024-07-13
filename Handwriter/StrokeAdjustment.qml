import QtQuick

import BGStudio.Handwriter
import BGStudio.Controls

FieldsGroupBox {
    id: strokeSettings
    property HWWriter writer
    /*property alias strokeColor: strokeColorSelector.strokeColor
    property alias strokeType: cbType.currentIndex
    property alias minPenSize: rsSize.first.value
    property alias strokeSize: rsSize.second.value
    property alias fadeoutLimit: rsFade.first.value
    property alias fadeinLimit: rsFade.second.value
    property alias velocityThreshold: sliVelocityThreshold.value*/

    /*property var writeParameter: {
        "strokeColor":strokeColorSelector.strokeColor,
        "strokeType": cbType.currentIndex,
        "minPenSize": rsSize.first.value,
        "strokeSize": Math.round(rsSize.second.value * 100) / 100,
        "fadeoutLimit": Math.min(rsFade.first.value, 1),
        "fadeinLimit": Math.max(rsFade.second.value, 1),
        "velocityThreshold": sliVelocityThreshold.value
    }*/

    signal activated()

    title: "Stroke Settings"

    FieldItem {
        label: "Type"
        field: "type"

        ComboBox {
            id: cbType
            model: ["Ballpoint", "Pen"]
            implicitWidth: 50

            onActivated: if (writer)
                             writer.strokeType = currentIndex
        }
    }

    FieldItem {
        label: "Color"

        StrokeColorSelectorComboBox {
            id: strokeColorSelector

            onActivated: if (strokeSettings.writer)
                             strokeSettings.writer.strokeColor = currentIndex
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

            first.onValueChanged: if (writer)
                                      writer.minPenSize = first.value
            second.onValueChanged: if (writer)
                                       writer.strokeSize = second.value

            // first.onMoved:  strokeSettings.activated()
            // second.onMoved: strokeSettings.activated()
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

            first.onValueChanged: if (writer)
                                      writer.fadeoutLimit = first.value
            second.onValueChanged: if (writer)
                                       writer.fadeinLimit = second.value

            // first.onMoved:  strokeSettings.activated()
            // second.onMoved: strokeSettings.activated()
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

            onValueChanged: if (writer)
                                writer.velocityThreshold = value

            // onMoved: strokeSettings.activated()
        }
    }

    /*function loadWriteParameter(param) {
        if (param) {
            strokeColorSelector.currentIndex = param.strokeColor
            cbType.currentIndex = param.strokeType
            rsSize.first.value = param.minPenSize
            rsSize.second.value = param.strokeSize
            rsFade.first.value = Math.min(param.fadeoutLimit, 1)
            rsFade.second.value = Math.max(param.fadeinLimit, 1)
            sliVelocityThreshold.value = param.velocityThreshold
        }
    }*/

    property Connections connections: Connections {
        target: writer || null
        function onStrokeColorChanged() {
            strokeColorSelector.currentIndex = writer.strokeColor
        }

        function onStrokeTypeChanged() {
            cbType.currentIndex = writer.strokeType
        }

        function onMinPenSizeChanged() {
            rsSize.first.value = writer.minPenSize
        }

        function onStrokeSizeChanged() {
            rsSize.second.value = writer.strokeSize
        }

        function onFadeoutLimitChanged() {
            rsFade.first.value = writer.fadeoutLimit
        }

        function onFadeinLimitChanged() {
            rsFade.second.value = writer.fadeinLimit
        }

        function onVelocityThresholdChanged() {
            sliVelocityThreshold.value = writer.velocityThreshold
        }
    }

    Component.onCompleted: {
        /*strokeColorSelector.currentIndex = writer.strokeColor
        cbType.currentIndex = writer.strokeType
        rsSize.first.value = writer.minPenSize
        rsSize.second.value = writer.strokeSize
        rsFade.first.value = Math.min(writer.fadeoutLimit, 1)
        rsFade.second.value = Math.max(writer.fadeinLimit, 1);
        sliVelocityThreshold.value = writer.velocityThreshold*/
    }
}
