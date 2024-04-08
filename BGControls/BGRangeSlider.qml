import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

import "Private" as P

T.RangeSlider {
    id: control

    readonly property int handleSize: 18
    readonly property int trackWidth: 14
    readonly property real trackLength: horizontal ? availableWidth - handleSize
                                                   : availableHeight - handleSize
    readonly property int indicatorMargin: 2
    readonly property bool overlapping: Math.abs(second.visualPosition - first.visualPosition) * trackLength < handleSize

    property color trackColor

    BGPaletteValue on trackColor {
        targetControl: control
        section: "default"
        stateMask: BGPaletteValue.STM_FORCE_UNFOCUS
    }

    BGControls.options: QtObject {
        property bool tick: false
    }

    implicitWidth: Math.max(
                       implicitBackgroundWidth + leftInset + rightInset,
                       first.implicitHandleWidth + leftPadding + rightPadding,
                       second.implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        implicitBackgroundHeight + topInset + bottomInset,
                        first.implicitHandleHeight + topPadding + bottomPadding,
                        second.implicitHandleHeight + topPadding + bottomPadding)

    padding: 5
    horizontalPadding: horizontal ? 0 : padding
    verticalPadding: horizontal ? padding : 0

    first.handle: P.BGSliderHandle {
        side: 0
        targetControl: control
        targetHandle: control.first
    }

    second.handle: P.BGSliderHandle {
        side: 1
        targetControl: control
        targetHandle: control.second
    }

    background: BGBorderBackground {
        x: control.horizontal ? (control.handleSize - control.trackWidth) / 2
                              : control.leftPadding
                                + (control.availableWidth - trackWidth) / 2
        y: control.horizontal ? control.topPadding
                                + (control.availableHeight - trackWidth) / 2
                              : (control.handleSize - control.trackWidth) / 2

        implicitWidth: control.horizontal ? 200 : trackWidth
        implicitHeight: control.horizontal ? trackWidth : 200

        width: control.horizontal ? control.availableWidth - x * 2
                                  : implicitWidth
        height: control.horizontal ? implicitHeight
                                   : control.availableHeight - y * 2

        shape: BGBorderBackground.RoundedRectangle
        style: BGBorderBackground.TwoDimensional
        cornerRadius: control.horizontal ? height / 2 : width / 2

        color: trackColor

        scale: control.horizontal && control.mirrored ? -1 : 1

        BGBorderBackground {
            shape: BGBorderBackground.RoundedRectangle
            border: false
            cornerRadius: control.horizontal ? height / 2 : width / 2

            x: control.horizontal ? control.indicatorMargin
                                    + control.first.visualPosition * control.trackLength
                                  : (parent.width - width) / 2
            y: control.horizontal ? (parent.height - height) / 2
                                  : control.indicatorMargin
                                    + control.second.visualPosition * control.trackLength

            width: control.horizontal ? (control.trackWidth - control.indicatorMargin * 2)
                                        + (control.second.visualPosition
                                           - control.first.visualPosition) * control.trackLength
                                      : parent.width - control.indicatorMargin * 2
            height: control.horizontal ? parent.height - control.indicatorMargin * 2
                                       : (control.trackWidth - control.indicatorMargin * 2)
                                         + (control.first.visualPosition
                                            - control.second.visualPosition) * control.trackLength

            BGPaletteValue on color {
                targetControl: control
                section: "default"
                stateMask: BGPaletteValue.STM_NONE | BGPaletteValue.STM_FORCE_HIGHLIGHT
            }

            opacity: 0.382 + (control.second.position - control.first.position) * 0.618
        }

        /*P.BGSliderTick {
            targetControl: control
            tickColor: trackColor
            tick: control.BGControls.options.tick
        }*/
    }

    P.BGSliderTick {
        targetControl: control
        tickColor: trackColor
        visible: control.BGControls.options.tick
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "default"
    }
}
