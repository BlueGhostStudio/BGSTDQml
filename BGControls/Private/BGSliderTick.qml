import QtQuick

Item {
    property Item targetControl
    property color tickColor

    x: (targetControl.width - width) / 2
    y: (targetControl.height - height) / 2
    width: targetControl.horizontal ? targetControl.trackLength
                                    : targetControl.availableWidth
    height: targetControl.horizontal ? targetControl.availableHeight
                                     : targetControl.trackLength
    z: -1

    Repeater {
        model: 11
        Item {
            property real tickLength: index % 5 ? targetControl.padding / 2
                                                : targetControl.padding
            x: targetControl.horizontal ? parent.width * index * 0.1 - 1 : 0
            y: targetControl.horizontal ? 0 : parent.height * index * 0.1 - 1
            width: targetControl.horizontal ? 2 : parent.width
            height: targetControl.horizontal ? parent.height : 2

            Rectangle {
                x: targetControl.horizontal ? 0 : -width
                y: targetControl.horizontal ? -height : 0
                width: targetControl.horizontal ? parent.width : tickLength
                height: targetControl.horizontal ? tickLength : parent.height
                color: tickColor
            }
            Rectangle {
                x: targetControl.horizontal ? 0 : parent.width
                y: targetControl.horizontal ? parent.height : 0
                width: targetControl.horizontal ? parent.width : tickLength
                height: targetControl.horizontal ? tickLength : parent.height
                color: tickColor
            }
        }
    }
}

/*Repeater {
    property Item targetControl
    property color tickColor
    property bool tick

    readonly property real interval: Math.abs(targetControl.background.width
                                              - targetControl.background.height) / 10
    readonly property real firstTick: targetControl.horizontal ? (targetControl.background.height - width) / 2
                                                               : (targetControl.background.width - height) / 2

    model: tick ? 11 :0

    Item {
        readonly property real tickLength: index % 5 ? targetControl.padding / 3 * 2
                                                     : targetControl.padding

        visible: tick

        x: targetControl.horizontal ? firstTick + index * interval
                                    : (parent.width - width) / 2
        y: targetControl.horizontal ? (parent.height - height) / 2
                                    : firstTick + index * interval

        width: targetControl.horizontal ? 1 : targetControl.availableWidth
        height: targetControl.horizontal ? targetControl.availableHeight : 1

        Rectangle {
            x: targetControl.horizontal ? 0 : -width
            y: targetControl.horizontal ? -height : 0
            width: targetControl.horizontal ? 1 : tickLength
            height: targetControl.horizontal ? tickLength : 1
            color: tickColor
        }

        Rectangle {
            x: targetControl.horizontal ? 0 : parent.width
            y: targetControl.horizontal ? parent.height : 0
            width: targetControl.horizontal ? 1 : tickLength
            height: targetControl.horizontal ? tickLength : 1
            color: tickColor
        }
    }
}*/
