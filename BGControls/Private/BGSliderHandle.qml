import QtQuick 2.15

import BGStudio.Controls

BGBorderBackground {
    property int side: -1
    property Item targetControl
    property var targetHandle
    property color handleColor

    BGPaletteValue on handleColor {
        targetControl: targetHandle.handle
        section: "default"
        stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS | BGPaletteValue.STM_FORCE_HIGHLIGHT
    }

    readonly property bool down: targetHandle.pressed
    readonly property bool hovered: targetHandle.hovered

    x: {
        var _x = targetControl.horizontal ? targetControl.leftPadding
                                            + targetHandle.visualPosition * targetControl.trackLength
                                          : targetControl.leftPadding
                                            + targetControl.availableWidth / 2
                                            - width / 2
        if (targetControl.overlapping === true
                && targetControl.horizontal
                && side !== -1)
            _x + (side == 0 ? -targetControl.handleSize / 2
                            : targetControl.handleSize / 2)
        else
            _x
    }
    y: {
        var _y = targetControl.horizontal ? targetControl.topPadding
                                            + targetControl.availableHeight / 2
                                            - height / 2
                                          : targetControl.topPadding
                                            + targetHandle.visualPosition * targetControl.trackLength
        if (targetControl.overlapping === true
                && !targetControl.horizontal
                && side != -1)
            _y + (side == 0 ? targetControl.handleSize / 2
                            : -targetControl.handleSize / 2)
        else
            _y
    }

    implicitWidth: targetControl.handleSize
    implicitHeight: targetControl.handleSize

    shape: {
        if (targetControl.overlapping !== undefined
                && side !== -1){
            if (side === 0)
                !targetControl.overlapping ? BGBorderBackground.Ellipse
                                           : (targetControl.horizontal ? BGBorderBackground.Lower
                                                                       : BGBorderBackground.LowerV)
            else
                !targetControl.overlapping ? BGBorderBackground.Ellipse
                                           : (targetControl.horizontal ? BGBorderBackground.Upper
                                                                       : BGBorderBackground.UpperV)
        } else
            BGBorderBackground.Ellipse
    }

    Rectangle {
        x: targetControl.horizontal ? (parent.width - width) / 2 : 3
        y: targetControl.horizontal ? 3 : (parent.height - height) / 2
        width: targetControl.horizontal ? 5 : parent.width - x * 2
        height: targetControl.horizontal ? parent.height - y * 2 : 5
        color: Qt.lighter(handleColor, 1.5)

        radius: 2

        Rectangle {
            x: 1
            y: 1
            width: parent.width - 2
            height: parent.height - 2
            color: Qt.darker(handleColor, 1.2)

            radius: 1
        }

        visible: targetControl.BGControls.options.tick && (side === -1 || !targetControl.overlapping)
    }

    color: handleColor
}
