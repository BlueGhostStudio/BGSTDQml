import QtQuick
import QtQuick.Effects
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.Dialog {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding,
                            implicitHeaderWidth,
                            implicitFooterWidth)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding
                             + (implicitHeaderHeight > 0 ? implicitHeaderHeight + spacing : 0)
                             + (implicitFooterHeight > 0 ? implicitFooterHeight + spacing : 0))

    padding: 6
    topPadding: title.length > 0 ? 0 : 3
    bottomPadding: topPadding
    spacing: 0

    BGControls.isContainer: true

    background: BGBorderBackground {
        id: borderBackground
        BGPaletteValue on color {
            targetControl: control
            section: "panel/VAR_POPUP"
        }

        shape: BGBorderBackground.RoundedRectangle
        cornerRadius: 6

        Rectangle {
            x: 0
            y: 0
            z: -1

            width: parent.width
            height: parent.height

            color: control.BGControls.accent
            radius: 6

            opacity: 0.25

            layer.enabled: true
            layer.effect: MultiEffect {
                blurEnabled: true
                blur: 1
            }
        }
    }

    header: Item {
        id: itHeaderWrap

        implicitHeight: rectHeaderBG.height + control.padding + 3
        visible: control.title.length > 0

        Rectangle {
            id: rectHeaderBG
            x: 3; y: 3
            width: parent.width - 6
            height: txtTitle.implicitHeight + control.padding * 2 - 6
            radius: 4

            BGControls.isContainer: true

            BGPaletteValue on color {
                targetControl: rectHeaderBG
                section: "panel/PART_TITLE"
            }

            Text {
                id: txtTitle
                x: control.padding - 3
                y: control.padding - 3
                width: parent.width - control.padding * 2 - 6

                text: control.title

                color: BGColor.contrast(parent.color)
                BGText.textScale: BGText.Subtitle1
            }
        }
    }

    footer: DialogButtonBox {
        padding: control.padding
        visible: count > 0
    }

    T.Overlay.modal: Rectangle {
        BGPaletteValue on color {
            targetControl: control
            paletteProperty: "custom"
            callback: () => {
                          return colorAdjustment("panel/VAR_POPUP/PART_OVERLAY/VAR_MODAL",
                                                 "background");
                      }
        }
    }

    T.Overlay.modeless: Rectangle {
        BGPaletteValue on color {
            targetControl: control
            paletteProperty: "custom"
            callback: () => {
                          return colorAdjustment("panel/VAR_POPUP/PART_OVERLAY/VAR_MODELESS",
                                                 "background")
                      }
        }
    }
}
