import QtQuick
import QtQuick.Controls

import BGStudio.Handwriter
import "LooseLeafPapers" as LLPs

StackView {
    id: stackview

    property LLPs.MainForm looseLeafPapersMainForm: LLPs.MainForm {}

    initialItem: Image {
        source: "qrc:/qt/qml/BGStudio/Handwriter/imgs/cover8.png"
        fillMode: Image.PreserveAspectCrop

        MouseArea {
            anchors.fill: parent

            onClicked: pushOrMove(looseLeafPapersMainForm)
        }

        StackView.onActivated: AndroidInterface.screenOrientation = 1
        StackView.onDeactivated: AndroidInterface.screenOrientation = -1
    }

    function pushOrMove(page) {
        let item = find(item => item === page)
        if (item) {
            var tails = []
            for (let i = item.StackView.index + 1; i < depth; i++) {
                tails.push(get(i))
            }
            if (tails.length > 0) {
                replace(item, tails, StackView.Immediate)
                push(page)
            }
        } else
            push(page)
    }
}
