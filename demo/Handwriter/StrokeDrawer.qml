import QtQuick
import QtQuick.Layouts

import BGStudio.Handwriter
import BGStudio.Controls

Drawer {
    id: drawer

    property int strokeColor: strokeColorSelector.currentIndex
    property var stroke: strokeSelector.currentValue

    x: (parent.width - width) / 2

    topPadding: 6
    edge: Qt.TopEdge
    dim: false

    contentItem: RowLayout {
        StrokeSelectorComboBox {
            id: strokeSelector
            implicitWidth: 120
        }
        StrokeColorSelectorComboBox {
            id: strokeColorSelector
        }
    }

    background: null
}
