import QtQuick
import BGStudio.Handwriter

Behavior {
    enabled: {
        const vp = targetProperty.object.HWViewport.viewport
        vp && vp.panAniEnabled
    }
    SequentialAnimation {
        PropertyAction {
            target: targetProperty.object.canvas
            property: "enabled"
            value: false
        }
        NumberAnimation { duration: 250 }
        PropertyAction {
            target: targetProperty.object.canvas
            property: "enabled"
            value: true
        }
    }
}
