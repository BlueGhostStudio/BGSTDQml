import QtQuick

import BGStudio.Controls
import QtQuick.Layouts

Pane {
    id: control
    default property alias fields: fieldsLayout.fields
    property alias fieldsLayout: fieldsLayout
    property alias flickable: flickable

    Layout.fillWidth: true

    implicitHeight: fieldsLayout.implicitHeight + topPadding + bottomPadding
    implicitWidth: fieldsLayout.implicitWidth + leftPadding + rightPadding

    padding: 8

    contentItem: Flickable {
        id: flickable

        clip: true
        contentHeight: fieldsLayout.implicitHeight
        contentWidth: fieldsLayout.implicitWidth

        flickableDirection: Flickable.AutoFlickIfNeeded
        boundsMovement: Flickable.StopAtBounds
        boundsBehavior: Flickable.DragAndOvershootBounds

        FieldsLayout {
            id: fieldsLayout
            width: Math.max(flickable.width, fieldsLayout.implicitWidth)
        }

        ScrollBar.vertical: ScrollBar {
            parent:  control
            anchors.right: parent.right
            anchors.top: flickable.top
            anchors.bottom: flickable.bottom
        }
        ScrollBar.horizontal: ScrollBar {
            parent:  control
            anchors.bottom: parent.bottom
            anchors.left: flickable.left
            anchors.right: flickable.right
        }
    }

    function fieldValue() {
        return fieldsLayout.values()
    }
}
