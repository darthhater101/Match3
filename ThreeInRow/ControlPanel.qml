import QtQuick 2.15
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    signal clicked
    property int movesCounter: 0
    property int scoreCounter: 0
    z: 1

    Rectangle {
        height: root.height
        color: root.color
        anchors.left: parent.left
        anchors.right: restartButton.left
        Text {
            anchors.centerIn: parent
            text: "Moves: " + root.movesCounter
            font.pixelSize: 22
        }
    }
    Rectangle {
        id: restartButton
        width: 120
        height: 40
        radius: 10
        color: area.pressed ? Qt.lighter(root.color, 1.2) : root.color
        border.width: 3
        border.color: "saddlebrown"
        anchors.centerIn: parent
        Text {
            text: "Restart"
            font.pixelSize: 22
            anchors.centerIn: parent
        }
        MouseArea {
            id: area
            anchors.fill: parent
            onClicked: root.clicked();
        }
    }
    Rectangle {
        height: root.height
        color: root.color
        anchors.right: parent.right
        anchors.left: restartButton.right
        Text {
            anchors.centerIn: parent
            text: "Score: " + root.scoreCounter
            font.pixelSize: 22
        }
    }
}
