import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.ComboBox {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        implicitBackgroundHeight + topInset + bottomInset,
                        implicitContentHeight + topPadding + bottomPadding,
                        implicitIndicatorHeight + topPadding + bottomPadding)

    spacing: 2
    padding: 2
    rightPadding: mirrored ? 2 : 24
    leftPadding: mirrored ? 24 : 2

    //horizontalPadding: 6
    //rightPadding: 6;
    indicator: ColorImage {
        x: control.mirrored ? (control.leftPadding + width) / 2 + 2 : control.width
                              - (control.rightPadding + width) / 2 - 2
        y: control.topPadding + (control.availableHeight - height) / 2
        color: control.BGButton.itemColor
        source: "icons/expand_more.png"
        sourceSize: "16x16"
    }

    contentItem: TextField {
        BGControls.options: {
            "transparent": !control.editable
        }

        text: control.editable ? control.editText : control.displayText

        BGText.textScale: control.editable ? BGText.Body1 : BGText.Button
        font: BGText.font
        color: control.editable ? inputForeground : control.BGButton.itemColor

        padding: 8
        leftPadding: control.editable ? padding : 8
        rightPadding: control.editable ? padding : 0

        enabled: control.editable
        BGPaletteValue on opacity {
            targetControl: contentItem
            paletteProperty: "custom"
            callback: function () {
                return 1;
            }
        }

        background.visible: control.editable
    }

    background: BGBorderBackground {
        implicitWidth: 64
        border: control.down ? false : true
        BGPaletteValue on color {
            targetControl: control
            section: "button"
            stateMask: BGPaletteValue.STM_ALL_EXCEPT_FOCUS
        }

        shape: BGBorderBackground.RoundedRectangle
    }

    delegate: ItemDelegate {
        required property var model
        required property int index

        BGButton.labelAlignment: Qt.AlignLeft

        width: ListView.view.width
        text: model[control.textRole]
        highlighted: control.highlightedIndex === index
        hoverEnabled: control.hoverEnabled

        Component.onCompleted: BGControls.forceRefresh()
    }

    popup: Popup {
        y: control.height
        width: control.width
        height: Math.min(
                    contentItem.implicitHeight + topPadding + bottomPadding,
                    control.Window.height - topMargin - bottomMargin)
        topMargin: 6
        bottomMargin: 6

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.highlightedIndex
            highlightMoveDuration: 0

            T.ScrollIndicator.vertical: ScrollIndicator {}
        }

        Component.onCompleted: BGControls.forceRefresh()
    }

    //opacity: BGPalette.enabledOpacity
    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "button"
    }
}
