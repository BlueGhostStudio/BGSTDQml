import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.MenuItem {
    id: control

    readonly property real arrowPadding: control.subMenu && control.arrow ? control.arrow.width + control.spacing : 0
    readonly property real indicatorPadding: control.checkable && control.indicator ? control.indicator.width + control.spacing : 0

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 6
    spacing: 2

    icon.width: 16
    icon.height: 16

    BGButton.isDelegate: true
    BGButton.labelAlignment: Qt.AlignLeft

    contentItem: control.BGButton.contentItem

    arrow: ColorImage {
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2

        visible: control.subMenu
        color: control.BGButton.itemColor
        source: "icons/chevron_right.png"
        sourceSize: "16x16"
    }

    indicator: ColorImage {
        x: control.mirrored ? control.width - width - control.rightPadding : control.leftPadding
        y: control.topPadding + (control.availableHeight - height) / 2

        //visible: control.checked
        color: control.BGButton.itemColor
        source: control.checkable ? (control.checked ? "icons/check.png" : "icons/check_box_unchecked.png") : ""
        sourceSize: "16x16"
    }

    background: BGBorderBackground {
        implicitWidth: 200
        implicitHeight: 20

        border: false
        BGPaletteValue on color {
            targetControl: control
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS & ~BGPaletteValue.STM_BACKGROUND
        }
        shape: BGBorderBackground.RoundedRectangle
        visible: control.down || control.highlighted
    }

    //opacity: control.BGPalette.enabledOpacity
    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "button"
    }

    Component.onCompleted: {
        contentItem.leftPadding = Qt.binding(() => {
                                                 return !control.mirrored ? indicatorPadding : arrowPadding
                                             })
        contentItem.rightPadding = Qt.binding(() => {
                                                  return control.mirrored ? indicatorPadding : arrowPadding
                                              })
    }
}
