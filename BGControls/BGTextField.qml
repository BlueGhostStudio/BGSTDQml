import QtQuick
import QtQuick.Templates as T
import BGStudio.Controls

T.TextField {
    id: control

    property color inputBackground
    property color inputForeground: {
        BGControls.options.transparent ? BGControls.foreground
                                       : BGColor.contrast(inputBackground)
    }
    property color inputBorder

    BGPaletteValue on inputBackground {
        targetControl: control
        paletteProperty: "custom"
        callback: () => {
                      if (control.BGControls.options.transparent) {
                          return Qt.color("transparent")
                      } else  {
                          var rule = BGPalette.selectRule("input/PART_BACKGROUND",
                                                          control.activeFocus
                                                          ? "focus"
                                                          : "background")

                          return BGPalette.colorAdjustment(
                              control.BGControls, rule,
                              control.BGControls.background)
                      }
                  }
    }

    BGPaletteValue on inputBorder {
        targetControl: control
        section: "input/PART_BORDER"
        stateMask: BGPaletteValue.STM_ALL & ~BGPaletteValue.STM_HIGHLIGHT
    }

    BGControls.options: QtObject {
        property bool transparent: false
    }

    implicitWidth: implicitBackgroundWidth + leftInset + rightInset || Math.max(
                       contentWidth,
                       placeholder.implicitWidth) + leftPadding + rightPadding
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    hoverEnabled: true
    placeholderTextColor: background.color

    BGText.textScale: BGText.Body1
    font: BGText.font

    selectionColor: BGControls.accent
    selectedTextColor: BGColor.contrast(BGControls.accent)
    color: inputForeground

    padding: 6
    leftPadding: padding
    rightPadding: padding
    topPadding: padding
    bottomPadding: padding

    Text {
        id: placeholder
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)

        text: control.placeholderText
        BGText.textScale: BGText.Button

        color: control.placeholderTextColor
        verticalAlignment: control.verticalAlignment
        visible: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
        elide: Text.ElideRight
    }

    cursorDelegate: Item {
        Rectangle {
            x: -1
            y: 0
            width: 2
            height: parent.height
            color: inputForeground

            opacity: 0.5
        }

        visible: control.activeFocus
        opacity: 0.5
    }

    background: BGBorderBackground {
        implicitWidth: 120

        color: control.inputBorder

        background2D: control.inputBackground
        style: BGBorderBackground.TwoDimensional
        shape: BGBorderBackground.RoundedRectangle
        cornerRadius: 3
        borderWidth: control.activeFocus ? 2 : 1
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "input"
    }
}
