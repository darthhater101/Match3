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

        property int previousClickedIndex: -1
        property int currentClickedIndex: -1
        property bool running: false

        model: field

        delegate: Tile {
            width: view.cellWidth
            height: view.cellHeight
            onClicked: {
                if(view.previousClickedIndex !== -1) {
                    view.currentClickedIndex = index;
                    if(!field.swap(view.currentClickedIndex, view.previousClickedIndex)) {
                        view.currentClickedIndex = -1;
                        view.previousClickedIndex = -1;
                    }
                }
                else {
                    view.previousClickedIndex = index;
                }
            }
        }

        Connections {
            onAnimationMoveEnded: update(running)
            onAnimationAddEnded: update(running)

            function update(running) {
                if(!running) {
                    if(field.checkForMatch()) {
                        field.removeMatched();
                        field.riseDeleted();
                        field.add();
                    }
                    else {
                        if(view.currentClickedIndex !== -1 || view.previousClickedIndex !== -1) {
                            field.swap(view.currentClickedIndex, view.previousClickedIndex);
                        }
                    }
                    view.currentClickedIndex = -1;
                    view.previousClickedIndex = -1;
                    if(!field.hasMoves()) {
                        console.log("No moves");
                    }
                }
            }
        }
    }
}
