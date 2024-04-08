import QtQuick
import QtQuick.Layouts

import BGStudio.Controls

FieldsGroupBox {
    id: fieldsGroupBox

    property color color: "green"
    property bool modified: false

    signal mixed

    component ColorSlider: Slider {
        id: slider
        from: 0
        to: 1

        FieldItem.fillWidth: false

        width: 200

        padding: 0
        horizontalPadding: 0

        onMoved: fieldsGroupBox.mixed()

        handle: Rectangle {
            x: slider.leftPadding + (slider.horizontal ? slider.visualPosition * (slider.availableWidth - width) : (slider.availableWidth - width) / 2)
            y: slider.topPadding + (slider.horizontal ? (slider.availableHeight - height) / 2 : sliderontrol.visualPosition * (slider.availableHeight - height))
            implicitWidth: 16
            implicitHeight: 16
            radius: 10

            color: {
                if (slider.FieldItem.field.field === "hue")
                    Qt.hsla(fieldsGroupBox.color.hslHue, 1, 0.5, 1)
                else
                    Qt.alpha(fieldsGroupBox.color, 0.75)
            }
            border.width: 2
            border.color: slider.BGControls.foreground
        }

        background: Canvas {
            x: slider.leftPadding + (slider.horizontal ? 0 : (slider.availableWidth - width) / 2)
            y: slider.topPadding + (slider.horizontal ? (slider.availableHeight - height) / 2 : 0)
            implicitWidth: slider.horizontal ? 200 : 10
            implicitHeight: slider.horizontal ? 10 : 200
            width: slider.horizontal ? slider.availableWidth : implicitWidth
            height: slider.horizontal ? implicitHeight : slider.availableHeight

            onPaint: {
                var ctx = getContext("2d")

                ctx.clearRect(0, 0, width, height)

                var h = fieldsGroupBox.color.hslHue
                var s = fieldsGroupBox.color.hslSaturation
                var l = fieldsGroupBox.color.hslLightness

                var genGradient = colorCmp => {
                    var gradient = ctx.createLinearGradient(8, 0, width - 8, 0)

                    for (var x = 0; x <= 1; x += 0.01) {
                        switch (slider.FieldItem.field.field) {
                        case "hue":
                            gradient.addColorStop(x,
                                                  Qt.color(Qt.hsla(x,
                                                                   1, 0.5, 1)))
                            break
                        case "saturation":
                            gradient.addColorStop(x, Qt.color(Qt.hsla(h,
                                                                      x, l, 1)))
                            break
                        case "lightness":
                            gradient.addColorStop(x, Qt.color(Qt.hsla(h,
                                                                      s, x, 1)))
                            break
                        }
                        // gradient.addColorStop(x, Qt.color(Qt.hsla(x, 1, 0.5, 1)));
                    }

                    return gradient
                }

                ctx.fillStyle = genGradient(1)
                ctx.beginPath()
                ctx.roundedRect(0, 0, width, height, 5, 5)
                ctx.fill()
            }

            scale: slider.horizontal && slider.mirrored ? -1 : 1

            Component.onCompleted: {
                fieldsGroupBox.colorChanged.connect(() => {
                                              requestPaint()
                                          })
            }
        }

        /*Component.onCompleted: {
            FieldItem.fillWidth = false
        }*/
    }

    FieldItem {
        id: fiColor
        label: "Color"

        Rectangle {
            implicitWidth: 200
            implicitHeight: 32
            FieldItem.fillWidth: false
            color: fieldsGroupBox.color
        }
    }

    FieldItem {
        label: "Hue"
        field: "hue"
        value: this.control.value
        ColorSlider {}
    }
    FieldItem {
        label: "Saturation"
        field: "saturation"
        value: this.control.value
        ColorSlider {}
    }
    FieldItem {
        label: "Lightness"
        field: "lightness"
        value: this.control.value
        ColorSlider {}
    }

    function reload() {
        fieldsGroupBox.fieldsLayout.control("hue").value = color.hslHue
        fieldsGroupBox.fieldsLayout.control("saturation").value = color.hslSaturation
        fieldsGroupBox.fieldsLayout.control("lightness").value = color.hslLightness
    }

    onMixed: {
        modified = true
        var colorValue = fieldsLayout.values()
        color = Qt.hsla(colorValue.hue, colorValue.saturation,
                        colorValue.lightness, 1)
    }

    onColorChanged: {
        if (!modified)
            reload()

        modified = false
    }

    Component.onCompleted: reload()
}
