import QtQuick
import QtQuick.Controls

import BGStudio.Controls
import BGStudio.Handwriter

TabBar {
    id: tabBar
    property LooseLeafViewport viewport

    position: TabBar.Footer

    Repeater {
        model: ActivePaperList.activePapersIndex

        TabButton {
            required property int pid
            required property int index
            text: "#" + pid
            checked: viewport && viewport.paper
                     && viewport.paper.localStorageID === pid

            onClicked: {
                ActivePaperList.openPaper(pid)
            }
        }
    }

    Connections {
        target: viewport || null
        function onPaperChanged() {
            if (!viewport.paper)
                tabBar.currentIndex = -1
        }
    }
}
