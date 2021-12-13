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

        model: field
        delegate: Tile {
            width: view.cellWidth
            height: view.cellHeight
            onClicked: {
                if(view.previousClickedIndex !== -1)
                {
                    field.swap(index, view.previousClickedIndex)
                    view.previousClickedIndex = -1;
                }
                else
                {
                    view.previousClickedIndex = index;
                }
            }
        }
    }
}
