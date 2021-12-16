import QtQuick 2.0

Item {
    id: root
    signal clicked
    visible: !deleted
    Rectangle {
        id: circle
        color: name
        width: parent.width - 30
        height: parent.height - 30
        radius: width * 0.5
        anchors.centerIn: parent
        border.color: "navy"
        border.width: match ? 5 : 0
        property point centerPoint: Qt.point(width / 2, height / 2)
        property alias wd: circle.width

        SequentialAnimation {
            id: anim
            NumberAnimation {
                target: circle
                properties: "scale"
                to: 1.2
                duration: 200
            }

            NumberAnimation {
                target: circle
                properties: "scale"
                to: 1.0
                duration: 200
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                var dist = Math.sqrt((circle.centerPoint.x - mouseX) * (circle.centerPoint.x - mouseX)
                                     + (circle.centerPoint.y - mouseY) * (circle.centerPoint.y - mouseY))
                if(dist <= circle.radius)
                {
                    anim.start()
                    root.clicked()
                }
            }
        }
    }
}
