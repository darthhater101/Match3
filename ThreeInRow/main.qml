import QtQuick 2.15
import QtQuick.Window 2.15

import GameFieldModel 1.0

Window {
    id: root
    width: field.columns * 100
    height: field.rows * 100
    visible: true
    title: qsTr("Hello World")
    color: "gray"

    GameFieldModel {
        id: field
    }

    GameBoard {
        id: view
        width: root.width
        height: root.height
        focus: true

        property int previousClickedIndex: -1
        property bool ended: false

        onAnimEnded: ended = isRun

        model: field
        delegate: Tile {
            width: view.cellWidth
            height: view.cellHeight
            onClicked: {
                if(view.previousClickedIndex !== -1)
                {
                    field.swap(index, view.previousClickedIndex)
                    console.log(view.ended)
                    while(view.moveAnimationRunning) {}
                    field.checkForMatch();
                    field.removeMatched();
                    field.update();
                    field.add();
                    view.previousClickedIndex = -1;
                }
                else
                {
                    view.previousClickedIndex = index;
                }
            }
        }

        Keys.onPressed: {
            if (event.key === Qt.Key_Q) {
                field.removeMatched()
            }
            if (event.key === Qt.Key_W) {
                field.update()
            }
            if (event.key === Qt.Key_E) {
                field.add()
            }
            if (event.key === Qt.Key_Escape) {
                root.close();
            }
        }
    }
}
