import QtQuick 2.15
import QtQuick.Window 2.15

import GameFieldModel 1.0

Window {
    id: root
    maximumWidth: (field.columns * 100) > 900 ? 900 : field.columns * 100
    maximumHeight: ((field.rows * 100) + 50) > 950 ? 950 : (field.rows * 100) + 50
    minimumWidth: maximumWidth
    minimumHeight: maximumHeight
    width: maximumWidth
    height: maximumHeight
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
        anchors.top: parent.top
        color: root.color
        scoreCounter: field.score
        movesCounter: field.moves
        onClicked: {
            losePopup.close();
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
        cellWidth: width < height ? height / field.rows : width / field.columns
        cellHeight: cellWidth

        property int holdIndex: -1
        property int targetIndex: -1

        model: field

        onAnimationAddEnded: {
            var wasMatched = false;
            for(var i = 0; i < viewIndexes.length; i++)
            {
                if(field.checkForMatchSmart(viewIndexes[i]))
                {
                    wasMatched = true;
                }
            }
            if(wasMatched)
            {
                field.removeMatched();
                field.riseDeleted();
                field.addNewTiles();
            }
            else
            {
                if(!field.hasMoves())
                {
                    losePopup.open();
                }
            }
        }

        onAnimationMoveEnded: {
            var wasMatched = false;
            for(var i = 0; i < viewIndexes.length; i++)
            {
                if(field.checkForMatchSmart(viewIndexes[i]))
                {
                    wasMatched = true;
                }
            }
            if(wasMatched)
            {
                field.removeMatched();
                field.riseDeleted();
                field.addNewTiles();
            }
        }

        function checkUserMove() {
            var viewIndexes = [view.targetIndex, view.holdIndex]
            var swapBack = true;

            for(var i = 0; i < viewIndexes.length; i++)
            {
                if(field.checkForMatchSmart(viewIndexes[i]))
                {
                    swapBack = false;
                }
            }
            if(swapBack)
            {
                field.moveSwap(targetIndex, holdIndex);
            }
            else
            {
                field.moves++;
                field.removeMatched();
                field.riseDeleted();
                field.addNewTiles();
            }
            targetIndex = -1;
            holdIndex = -1;
        }


        delegate: Tile {
            id: tile

            width: view.cellWidth
            height: view.cellHeight

            onEntered: {
                view.holdIndex = holdIndex;
                view.targetIndex = targetIndex;
            }

            onDropped: {
                field.swap(view.holdIndex, view.targetIndex);
                view.checkUserMove();
            }
        }
    }
}
