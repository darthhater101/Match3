import QtQuick 2.0

GridView {
    id: root
    interactive: false
    move: moveAnimation
    displaced: moveAnimation
    add: addAnimation

    signal animationMoveEnded(bool running)
    signal animationAddEnded(bool running)

    Transition {
        id: addAnimation
        NumberAnimation {
            property: "y"; from: -100; duration: 400
        }
        onRunningChanged: animationAddEnded(running)
    }

    Transition {
        id: moveAnimation
        NumberAnimation {
            properties: "x, y"; duration: 400;
        }
        onRunningChanged: animationMoveEnded(running)
    }

}
