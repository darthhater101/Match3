import QtQuick 2.15

Item {
    id: root
    signal clicked
    signal dropped()
    signal entered(int holdIndex, int targetIndex)
    property Item currentItem: null
    property bool isHorizontal: false

    visible: !deleted

    DropArea {
        id: dropArea
        anchors.fill: parent
        onEntered: {
            if(drag.source.rectIndex !== index)
            {
                circle.animationEnabled = true
                var offset = 0;

                if(Math.abs(drag.source.rectIndex - index) === 1)
                {
                    offset = root.width
                    if(drag.source.rectIndex < index)
                    {
                        offset = -offset
                    }
                    circle.anchors.horizontalCenterOffset = offset
                    isHorizontal = true
                }
                else
                {
                    offset = root.height
                    if(drag.source.rectIndex < index)
                    {
                        offset = -offset
                    }
                    circle.anchors.verticalCenterOffset = offset
                    isHorizontal = false
                }
                root.currentItem = circle
                root.entered(drag.source.rectIndex, index)
            }
            else
            {
                root.entered(drag.source.rectIndex, index)
            }
        }
        onDropped: {
            if(root.currentItem)
            {
                root.currentItem.animationEnabled = false
                if(isHorizontal)
                {
                    root.currentItem.anchors.horizontalCenterOffset = 0
                }
                else
                {
                    root.currentItem.anchors.verticalCenterOffset = 0
                }
                root.currentItem = null
            }
        }

        onExited: {
            if(root.currentItem)
            {
                if(isHorizontal)
                {
                    root.currentItem.anchors.horizontalCenterOffset = 0
                }
                else
                {
                    root.currentItem.anchors.verticalCenterOffset = 0
                }
                root.currentItem = null
            }
        }
    }



    Rectangle {
        id: circle
        color: name
        width: root.width * 0.7
        height: root.height * 0.7
        radius: width * 0.5
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        property point centerPoint: Qt.point(width / 2, height / 2)
        property int rectIndex: index
        property bool animationEnabled: true

        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.lighter(circle.color, 1.5) }
            GradientStop { position: 0.2; color: circle.color }
            GradientStop { position: 1.0; color: Qt.darker(circle.color, 4.0) }
        }

        states: [
            State {
                name: "drag"
                when: dragArea.drag.active
                AnchorChanges {
                    target: circle
                    anchors.verticalCenter: undefined
                    anchors.horizontalCenter: undefined
                }
            }
        ]

        Behavior on anchors.horizontalCenterOffset {
            enabled: circle.animationEnabled
            NumberAnimation {
                duration: 400
            }
        }

        Behavior on anchors.verticalCenterOffset {
            enabled: circle.animationEnabled
            NumberAnimation {
                duration: 400
            }
        }

        Drag.active: dragArea.drag.active
        Drag.hotSpot.x: 25
        Drag.hotSpot.y: 25

    }

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: circle
        drag.maximumX: root.width + (root.width * 0.3) / 2
        drag.minimumX: -root.width + (root.width * 0.3) / 2
        drag.maximumY: root.height + (root.height * 0.3) / 2
        drag.minimumY: -root.height + (root.height * 0.3) / 2

        property bool axisCanBeChanged: true
        property int shiftX: 0
        property int shiftY: 0
        property int startX: 0
        property int startY: 0

        onMouseXChanged: shiftX = mouseX - startX
        onMouseYChanged: shiftY = mouseY - startY
        onPositionChanged: {
            if(axisCanBeChanged)
            {
                if(Math.abs(shiftX) > Math.abs(shiftY))
                {
                    drag.axis = Drag.XAxis;
                }
                else if(Math.abs(shiftY) > Math.abs(shiftX))
                {
                    drag.axis = Drag.YAxis;
                }
                axisCanBeChanged = false;
            }
        }

        onPressed: {
            startX = mouseX;
            startY = mouseY;
        }

        onReleased: {
            circle.Drag.drop();
            root.dropped();
            shiftX = 0;
            shiftY = 0;
            axisCanBeChanged = true;
        }
    }
}
