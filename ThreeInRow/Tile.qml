import QtQuick 2.15

Item {
    id: root
    signal clicked

    property Item dragParent: undefined
    property Item areaParent: undefined
    property int visualIndex: 0
    property alias color: circle.color
    property bool deleted: false
    //visible: !deleted

    DragHandler {
        id: dragHandler
        yAxis.minimum: areaParent.y - root.height
        yAxis.maximum: areaParent.y + root.height
        xAxis.minimum: areaParent.x - root.width
        xAxis.maximum: areaParent.x + root.width
    }

    Drag.active: dragHandler.active
    Drag.source: root
    Drag.hotSpot.x: 36
    Drag.hotSpot.y: 36

    states: [
        State {
            when: dragHandler.active
            ParentChange {
                target: root
                parent: root.dragParent
            }

            AnchorChanges {
                target: root
                anchors.horizontalCenter: undefined
                anchors.verticalCenter: undefined
            }

            PropertyChanges {
                target: circle
                scale: 1.2
            }
        }
    ]

    Rectangle {
        id: circle
        color: name
        width: parent.width * 0.7
        height: parent.height * 0.7
        radius: width * 0.5
        anchors.centerIn: parent
        property point centerPoint: Qt.point(width / 2, height / 2)


        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.lighter(circle.color, 1.5) }
            GradientStop { position: 0.2; color: circle.color }
            GradientStop { position: 1.0; color: Qt.darker(circle.color, 4.0) }
        }

        Behavior on scale {
            NumberAnimation {
                duration: 200
            }
        }

//        MouseArea {
//            anchors.fill: parent
//            onClicked: {
//                var dist = Math.sqrt((circle.centerPoint.x - mouseX) * (circle.centerPoint.x - mouseX)
//                                     + (circle.centerPoint.y - mouseY) * (circle.centerPoint.y - mouseY))
//                if(dist <= circle.radius)
//                {
//                    root.clicked()
//                }
//            }
//            onPressed: circle.scale = 1.2
//            onReleased: circle.scale = 1.0
//        }
    }
}
