import QtQuick 2.0

GridView {
    id: root
    interactive: false
    move: moveAnimation
    displaced: moveAnimation
    add: addAnimation

    signal animationAddEnded(variant viewIndexes)
    signal animationMoveEnded(variant viewIndexes)

    property alias moveAnimationEnabled: moveAnimation.enabled

    Transition {
        id: addAnimation
        NumberAnimation {
            property: "y"; from: -100; duration: 400
        }
        onRunningChanged: {
            if(!running) {
                animationAddEnded(ViewTransition.targetIndexes);
            }
        }
    }

    Transition {
        id: moveAnimation
        NumberAnimation {
            properties: "x, y"; duration: 400;
        }
        onRunningChanged: {
            if(!running) {
                animationMoveEnded(ViewTransition.targetIndexes);
            }
        }
    }

}
