import QtQuick

import BGStudio.Controls
import QtQuick.Layouts

//import QtQuick.Controls
FieldItem {
    FieldsGroupBox {
        title: "Slider Demo"
        DemoCtrlGroup {
            ctrlCmp: Slider {
                BGControls.options: {
                    "tick": true
                }

                orientation: Qt.Vertical
            }
            model: [{
                    "enabled": true
                }, {
                    "enabled": false
                }]
        }
        DemoCtrlGroup {
            ctrlCmp: RangeSlider {
                BGControls.options: {
                    "tick": true
                }

                //orientation: Qt.Vertical
            }
            model: [{
                    "enabled": true
                }, {
                    "enabled": false
                }]
        }
    }
}
