import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

import mf.components 1.0
import "Components"
import Theme 1.0

Window {
    visible:true
    width:480
    height:480
    title: "Test application"
    id:wnd

    color: Theme.background_color

    ColumnLayout {
        width: wnd.width
        spacing: 0
        anchors.fill: parent
        RowLayout {
            Layout.margins: 10
            spacing:4
            Button {
                text: "Refresh"
                onClicked: BackEnd.appManager.refresh()
            }
            Button {
                text: "Set Hooks"
                onClicked: BackEnd.appManager.installHooks()
            }
            Button {
                text: "Remove Hooks"
                onClicked: BackEnd.appManager.uninstallHooks()
            }
        }
        WindowList {
            Layout.margins: 10
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
        Rectangle {
            Layout.preferredWidth: 200
            Layout.preferredHeight: 50
            Layout.margins: 10
            Layout.alignment: Qt.AlignHCenter
            color: BackEnd.color
        }
    }
}
