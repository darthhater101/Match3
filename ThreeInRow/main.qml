import QtQuick 2.15
import QtQuick.Window 2.15

import GameFieldModel 1.0

Window {
    id: root
    maximumWidth: (field.columns * 100) > 900 ? 900 : field.columns * 100
    maximumHeight: ((field.rows * 100) + 50) > 950 ? 950 : (field.rows * 100) + 50
    minimumWidth: maximumWidth
    minimumHeight: maximumHeight
    visible: true
    title: qsTr("Hello World")
    color: "burlywood"

    GameFieldModel {
        id: field
        onScoreChanged: controlPanel.scoreCounter = score
    }

    ControlPanel {
        id: controlPanel
        width: parent.width
        height: 50
        color: root.color
        scoreCounter: field.score
        onClicked: {
            losePopup.close();
            view.movesCounter = 0
            field.generateBoard();
        }
    }

    LosePopup {
        id: losePopup
        anchors.centerIn: parent
        color: root.color
    }

    GameBoard {
        id: view
        anchors.top: controlPanel.bottom
        anchors.bottom: parent.bottom
        width: root.width
        cellWidth: width / field.columns
        cellHeight: height / field.rows

        property int previousClickedIndex: -1
        property int currentClickedIndex: -1
        property bool running: false
        property int movesCounter: 0

        model: field

        onAnimationMoveEnded: update(running)
        onAnimationAddEnded: update(running)

        delegate: Tile {
            width: view.cellWidth
            height: view.cellHeight
            enabled: !losePopup.opened
            onClicked: {
                if(view.previousClickedIndex !== -1) {
                    view.currentClickedIndex = index;
                    if(!field.swap(view.currentClickedIndex, view.previousClickedIndex)) {
                        view.currentClickedIndex = -1;
                        view.previousClickedIndex = -1;
                    }
                    else {
                        view.movesCounter++;
                    }
                }
                else {
                    view.previousClickedIndex = index;
                }
            }
        }

        function update(running) {
            if(!running) {
                if(field.checkForMatch()) {
                    controlPanel.movesCounter = movesCounter
                    field.removeMatched();
                    field.riseDeleted();
                    field.addNewTiles();
                }
                else {
                    if(view.currentClickedIndex !== -1 || view.previousClickedIndex !== -1) {
                        field.swap(view.currentClickedIndex, view.previousClickedIndex);
                    }
                }
                view.currentClickedIndex = -1;
                view.previousClickedIndex = -1;
                if(!field.hasMoves()) {
                    losePopup.open();
                }
            }
        }
    }
}
