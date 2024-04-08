import QtQuick

import BGStudio.Controls
import QtQuick.Layouts

//import QtQuick.Controls
DemoCtrlGroup {
    columns: 2

    ctrlCmp: Frame {
        width: 250

        Text {
            text: "Frame"
        }
    }

    model: [{
            "enabled": true
        }, {
            "enabled": false
        }]
}
