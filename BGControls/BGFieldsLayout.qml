import QtQuick
import QtQuick.Layouts

import BGStudio.Controls

GridLayout {
    id: fieldsLayout
    default property list<FieldItem> fields:[]

    //uniformCellWidths: false
    columnSpacing: 5
    rowSpacing: 6

    columns: 2

    function fieldValue(field) {
        for(var x in fields) {
            if (fields[x].field ===  field) {
                var v = fields[x].value
                break
            }
        }

        return v
    }

    function control(field) {
        for (var x in fields) {
            if (fields[x].field === field)
                return fields[x].control
        }

        return undefined
    }

    function values() {
        var v = {}
        for (var x in fields) {
            var field = fields[x].field
            if (field)
                v[field] = fields[x].value
        }

        return v;
    }

    Component.onCompleted: {
        var row = 0
        for (var x in fields) {
            var control = fields[x].control
            var controlWrapper = fields[x].controlWrapper
            controlWrapper.parent = fieldsLayout

            if (fields[x].fillWidth)
                control.anchors.fill = controlWrapper

            if (fields[x].label.length > 0) {
                var label = fields[x].labelCtrl
                label.parent = fieldsLayout
                label.Layout.row = row
                label.Layout.column = 0

                if (fields[x].orientation === Qt.Horizontal) {
                    label.Layout.alignment = Qt.AlignRight
                    controlWrapper.Layout.column = 1
                } else {
                    label.Layout.alignment = Qt.AlignLeft
                    label.Layout.columnSpan = 2
                    controlWrapper.Layout.column = 0
                    controlWrapper.Layout.topMargin = -6
                    controlWrapper.Layout.columnSpan = 2
                    row++
                }
            } else {
                controlWrapper.Layout.column = 0
                controlWrapper.Layout.columnSpan = 2
            }

            controlWrapper.Layout.row = row
            row++
        }
    }
}
