import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.12

Rectangle {
    id: root
    signal clicked
    property int movesCounter: 0
    property int scoreCounter: 0
    z: 1

    RowLayout {
        id: row
        property int margins: 50
        anchors.fill: parent
        Text {
            anchors.right: restartButton.left
            anchors.rightMargin: row.margins
            text: "Moves: " + root.movesCounter
            font.pixelSize: 22
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
        Text {
            anchors.left: restartButton.right
            anchors.leftMargin: row.margins
            text: "Score: " + root.scoreCounter
            font.pixelSize: 22
        }
    }
}
