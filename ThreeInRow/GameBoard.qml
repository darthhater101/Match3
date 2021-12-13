import QtQuick 2.0

GridView {
    id: root
    interactive: false
    cellWidth: width / field.columns
    cellHeight: height / field.rows
    move: animation
    displaced: animation

    Transition {
        id: animation
        NumberAnimation {
            properties: "x, y"; duration: 400;
        }
    }

}
