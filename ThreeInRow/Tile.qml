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
        border.width: possible ? 5 : 0
        property point centerPoint: Qt.point(width / 2, height / 2)

        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.lighter(circle.color, 1.5) }
            GradientStop { position: 0.2; color: circle.color }
            GradientStop { position: 1.0; color: Qt.darker(circle.color, 4.0) }
        }

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
