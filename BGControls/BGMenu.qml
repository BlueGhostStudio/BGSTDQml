import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.Menu {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    margins: 0
    padding: 6
    overlap: 2
    spacing: 2

    BGControls.isContainer: true

    delegate: MenuItem { }

    contentItem: ListView {
        clip: true
        implicitHeight: contentHeight
        model: control.contentModel
        currentIndex: control.currentIndex
        highlightMoveDuration: 0
        interactive: Window.window
                        ? contentHeight + control.topPadding + control.bottomPadding > Window.window.height
                        : false

        T.ScrollIndicator.vertical: ScrollIndicator {}
    }

    background: BGBorderBackground {
        implicitWidth: 200
        implicitHeight: 20

        //color: control.BGControls.background//control.BGPalette.mixColor("popup")
        BGPaletteValue on color {
            targetControl: control
            section: "panel"
        }

        shape: BGBorderBackground.RoundedRectangle
        cornerRadius: 6
    }

    readonly property color overlayColor: BGColor.brightnessInversion(control.BGControls.background, 0.75)

    T.Overlay.modal: Rectangle {
        color: Color.transparent(overlayColor, 0.5)
    }

    T.Overlay.modeless: Rectangle {
        color: Color.transparent(overlayColor, 0.12)
    }
}
