import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import BGStudio.Controls

T.TabBar {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    BGControls.isContainer: true
    BGControls.role: control.position === T.TabBar.Header ? BGControls.PRIMARY : BGControls.BASE

    padding: 2
    leftPadding: 1
    rightPadding: 1
    spacing: 0

    contentItem: ListView {
        model: control.contentModel
        currentIndex: control.currentIndex

        spacing: control.spacing
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.AutoFlickIfNeeded
        snapMode: ListView.SnapToItem


        /*highlightMoveDuration: 0
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: 40
        preferredHighlightEnd: width - 40*/
    }

    background: Rectangle {
        /*color: control.position === T.TabBar.Header ? control.BGControls.background
                                                    : "transparent"*/
        BGPaletteValue on color {
            targetControl: control
            paletteProperty: "custom"
            section: "panel"
            callback: () => {
                          return control.position === T.TabBar.Header ?
                              BGPalette.mixColor(section, control.BGControls,
                                                 true, false, false, false,
                                                 BGPalette.PALETTE_UNSET) :
                              "transparent"
                      }
        }
        /*BGPaletteValue on color {
            control: control
            section: "panel"
        }*/
    }
}
