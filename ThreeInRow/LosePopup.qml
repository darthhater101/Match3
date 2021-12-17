import QtQuick 2.15
import QtQuick.Controls 2.12

Popup {
    id: root
    width: 300
    height: 150
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    property alias color: background.color

    background: Rectangle {
        id: background
        radius: 10
        border.width: 5
        border.color: "saddlebrown"
    }

    Text {
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
        text: "No more moves!\nYou can restart the game!"
        font.pixelSize: 22
    }
}
