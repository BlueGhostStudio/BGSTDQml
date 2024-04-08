import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.GroupBox {
    id: control

    enum BorderType {
        NoBorder,
        Border,
        OnlyTopLevel
    }

    BGControls.isContainer: true
    BGControls.options: QtObject {
        property bool levelCounter: true
        property  int level: -1
        property  int border: GroupBox.Border
        property bool noTitleBackground: false
        property bool transparent: false
        property bool shade: false
        property  int role: BGControls.ACCENT
    }

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding,
                            implicitLabelWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    spacing: 0
    padding: 6
    topPadding: title.length > 0 ?
                    implicitLabelHeight + 3 + padding :
                    padding + 2
    leftPadding: padding + 2
    rightPadding: leftPadding
    bottomPadding: padding + 2

    /*label: Text {
        x: control.padding
        y: control.padding
        width: control.availableWidth

        text: control.title
        elide: Text.ElideRight

        BGText.textScale: {
            switch (control.BGControls.options.level) {
            case 1:
                BGText.H5 //21.5
                break
            case 2:
                BGText.H6 // 12.5
                break
            case 3:
                BGText.Subtitle1 // 10
                break
            case 4:
                BGText.Caption // 7.5
                break
            default:
                BGText.Overline // 6.25
            }
        }
    }*/
    label: Canvas {
        id: cvLabel

        property color color

        x: 3
        y: 3
        implicitWidth: txtLabel.implicitWidth + control.padding * 2
        width: control.availableWidth + leftPadding + rightPadding - 10
        implicitHeight: txtLabel.implicitHeight
                        + (control.BGControls.options.noTitleBackground
                           ? 0 : control.padding)
        visible: title.length > 0

        BGControls.isContainer: true

        BGPaletteValue on color {
            targetControl: cvLabel
            section: control.BGControls.options.noTitleBackground ?
                         "panel/PART_TITLE/VAR_NO_BACKGROUND" :
                         "panel"
                         + (control.BGControls.options.role === BGControls.PRIMARY ? "/VAR_PRIMARY" : "")
                         + "/PART_TITLE"
        }

        Text {
            id: txtLabel

            //anchors.verticalCenter: parent.verticalCenter

            // x: control.padding
            // y: control.BGControls.options.noTitleBackground ? 0 : control.padding / 2
            anchors.fill: parent
            leftPadding: control.padding
            rightPadding: control.padding
            verticalAlignment: Text.AlignVCenter

            width: parent.width - control.padding * 2

            text: control.title
            color: cvLabel.BGControls.foreground
            elide: Text.ElideRight

            BGText.textScale: {
                switch (control.BGControls.options.level) {
                case 1:
                    BGText.H5 //21.5
                    break
                case 2:
                    BGText.H6 // 12.5
                    break
                case 3:
                    BGText.Subtitle1 // 10
                    break
                case 4:
                    BGText.Caption // 7.5
                    break
                default:
                    BGText.Overline // 6.25
                }
            }
        }

        onPaint: {
            var ctx = getContext("2d")

            ctx.clearRect(0, 0, width, height)

            if (!control.BGControls.options.noTitleBackground) {
                var gradient = ctx.createLinearGradient(0, 0, width, height)
                gradient.addColorStop(0, cvLabel.color)
                gradient.addColorStop(1, Qt.alpha(cvLabel.color, 0))

                ctx.fillStyle = gradient
                ctx.beginPath()
                ctx.fillRect(0, 0, width, height)
            }
        }

        onColorChanged: requestPaint()
    }

    background: BGBorderBackground {
        BGPaletteValue on color {
            targetControl: control
            section: control.BGControls.options.transparent ?
                         (control.BGControls.options.shade ?
                              "panel":
                              "panel/VAR_NOSHADE") :
                         "panel/VAR_SHADE"
        }

        shape: BGBorderBackground.RoundedRectangle
        cornerRadius: 3
        border: {
            if (control.BGControls.options.border === GroupBox.NoBorder)
                false
            else if (control.BGControls.options.border === GroupBox.Border)
                true
            else
                control.BGControls.options.level === 1
        }
    }

    BGPaletteValue on opacity {
        targetControl: control
        paletteProperty: "enable"
        section: "panel"
    }

    BGControls.onOptionsChanged: {
        updateOptions()
        cvLabel.requestPaint()
    }

    function updateOptions() {
        if (!BGControls.options.levelCounter)
            BGControls.options.level = -1
        else {
            var level = 0
            BGControls.backtrackContainer((item, control) => {
                                              if (control
                                                  && control.options.levelCounter) {
                                                  if (control.options.level === -1) {
                                                      level++
                                                      return false
                                                  } else {
                                                      level += control.options.level
                                                      return true
                                                  }
                                              }
                                          })
            BGControls.options.level = level
        }
    }

    //Component.onCompleted: updateOptions()
}
