import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

import "Private" as P

T.Slider {
    id: control

    readonly property int handleSize: 18
    readonly property int trackWidth: 14
    readonly property real trackLength: horizontal ? availableWidth - handleSize
                                                   : availableHeight - handleSize
    readonly property int indicatorMargin: 2

    property color trackColor

    BGPaletteValue on trackColor {
        targetControl: control
        section: "default"
        stateMask: BGPaletteValue.STM_FORCE_UNFOCUS
    }

    BGControls.options: QtObject {
        property bool tick: false
    }

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitHandleWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitHandleHeight + topPadding + bottomPadding)

    padding: 5
    horizontalPadding: horizontal ? 0 : padding
    verticalPadding: horizontal ? padding : 0

    handle: P.BGSliderHandle {
        targetControl: control
        targetHandle: control
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

        color: control.trackColor

        scale: control.horizontal && control.mirrored ? -1 : 1

        BGBorderBackground {
            shape: BGBorderBackground.RoundedRectangle
            border: false
            cornerRadius: control.horizontal ? height / 2 : width / 2

            x: control.indicatorMargin
            y: control.horizontal ? (parent.height - height) / 2
                                  : control.indicatorMargin
                                    + control.visualPosition * control.trackLength

            width: control.horizontal ? (control.trackWidth - control.indicatorMargin * 2)
                                        + control.visualPosition * control.trackLength
                                      : parent.width - control.indicatorMargin * 2
            height: control.horizontal ? parent.height - control.indicatorMargin * 2
                                       : (control.trackWidth - control.indicatorMargin * 2)
                                         + (1 - control.visualPosition) * control.trackLength

            BGPaletteValue on color {
                targetControl: control
                section: "default"
                stateMask: BGPaletteValue.STM_NONE | BGPaletteValue.STM_FORCE_HIGHLIGHT
            }

            opacity: 0.382 + control.position * 0.618
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
