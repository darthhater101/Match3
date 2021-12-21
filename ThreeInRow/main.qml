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
        onMovesChanged: controlPanel.movesCounter = moves
    }

    ControlPanel {
        id: controlPanel
        width: parent.width
        height: 50
        color: root.color
        scoreCounter: field.score
        movesCounter: field.moves
        onClicked: {
            losePopup.close();
            field.moves = 0;
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

        model: field

        onAnimationAddEnded: {
            for(var i = 0; i < viewIndexes.length; i++)
            {
                field.checkForMatch2(viewIndexes[i]);
            }
            field.removeMatched();
            field.riseDeleted();
            field.addNewTiles();
        }
        onAnimationMoveEnded: {
            if(viewIndexes.length === 1)
            {
                viewIndexes.push(viewIndexes[0] - 1);
            }

            var swapBack = true;

            for(var i = 0; i < viewIndexes.length; i++)
            {
                if(field.checkForMatch2(viewIndexes[i]))
                {
                    swapBack = false;
                }
            }
            if(swapBack && previousClickedIndex !== -1 && currentClickedIndex !== -1)
            {
                field.swap(currentClickedIndex, previousClickedIndex);
            }
            else
            {
                field.moves++;
            }

            field.removeMatched();
            field.riseDeleted();
            field.addNewTiles();
            previousClickedIndex = -1;
            currentClickedIndex = -1;
        }

        delegate: Tile {
            width: view.cellWidth
            height: view.cellHeight
            enabled: !losePopup.opened
            onClicked: {
                if(view.currentClickedIndex === -1)
                {
                    view.currentClickedIndex = index;
                }
                else if(view.previousClickedIndex === -1)
                {
                    view.previousClickedIndex = view.currentClickedIndex;
                    view.currentClickedIndex = index;
                }

                if(view.currentClickedIndex !== -1 && view.previousClickedIndex !== -1)
                {
                    if(!field.swap(view.currentClickedIndex, view.previousClickedIndex)) {
                        view.currentClickedIndex = -1;
                        view.previousClickedIndex = -1;
                    }
                }
            }
        }
    }
}
