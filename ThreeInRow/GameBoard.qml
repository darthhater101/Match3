import QtQuick 2.0

GridView {
    id: root
    interactive: false
    cellWidth: width / field.columns
    cellHeight: height / field.rows
    move: moveAnimation
    displaced: moveAnimation
    add: addAnimation

    property alias moveAnimationRunning: moveAnimation.running

    signal animEnded(bool isRun)

    Transition {
        id: addAnimation
        NumberAnimation {
            property: "y"; from: -100; duration: 400
        }
    }

    Transition {
        id: moveAnimation
        NumberAnimation {
            properties: "x, y"; duration: 400;
        }
        onRunningChanged: animEnded(running)
    }

}
