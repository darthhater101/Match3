#include "gamefieldmodel.h"

#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>


QColor GameConfig::getRandomColor()
{
    return colors[rand() % colors.size()];
}

GameFieldModel::GameFieldModel(QObject *parent)
    : QAbstractListModel(parent), m_score(0), m_moves(0)
{
    m_roleNames[ColorRole] = "name";
    m_roleNames[DeletedRole] = "deleted";
    m_roleNames[MatchedRole] = "matched";

    m_gameConfig = loadGameFieldConfiguration();
    m_gameField.resize(m_gameConfig.rows * m_gameConfig.columns);

    generateBoard();
}

GameFieldModel::~GameFieldModel()
{
}

int GameFieldModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_gameField.count();
}

QVariant GameFieldModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    if(row < 0 || row >= m_gameField.count())
    {
        return QVariant();
    }

    switch (role)
    {
    case ColorRole:
        return m_gameField[row].getColor().name();
    case DeletedRole:
        return m_gameField[row].isDeleted();
    case MatchedRole:
        return m_gameField[row].isMatched();
    }

    return QVariant();
}

bool GameFieldModel::moveSwap(int from, int to)
{
    if((std::abs(from - to) == 1 && std::floor(from / m_gameConfig.columns) == std::floor(to / m_gameConfig.columns))
            || std::abs(from - to) == m_gameConfig.columns)
    {
        int min = std::min(from, to);
        int max = std::max(from, to);

        if(beginMoveRows(QModelIndex(), min, min, QModelIndex(), max + 1))
        {
            m_gameField.move(min, max);
            endMoveRows();
        }
        if(beginMoveRows(QModelIndex(), max - 1, max - 1, QModelIndex(), min))
        {
            m_gameField.move(max - 1, min);
            endMoveRows();
        }

        return true;
    }

    return false;
}

bool GameFieldModel::swap(int from, int to)
{
    if((std::abs(from - to) == 1 && std::floor(from / m_gameConfig.columns) == std::floor(to / m_gameConfig.columns))
            || std::abs(from - to) == m_gameConfig.columns)
    {
        m_gameField.swapItemsAt(from, to);
        emit dataChanged(createIndex(0, 0), createIndex(m_gameField.size(), 0));
        return true;
    }
    return false;
}

GameConfig GameFieldModel::loadGameFieldConfiguration()
{
    QFile configFile("://config.json");
    QString content;
    if(configFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        content = configFile.readAll();
        configFile.close();
    }
    else
    {
        QCoreApplication::exit(1);
    }

    QJsonDocument doc = QJsonDocument::fromJson(content.toUtf8());

    QJsonArray arr = doc.object().value(QString("colors")).toArray();
    int rows = doc.object().value(QString("rows")).toInt();
    int columns = doc.object().value(QString("columns")).toInt();

    rows = rows < 4? 4 : rows > 20 ? 20 : rows;
    columns = columns < 4 ? 4 : columns > 20 ? 20 : columns;

    if(arr.size() < 3)
    {
        QString colors[3] = { "red", "green", "blue" };
        for(int i = 0; i < 3; i++)
        {
            if(!arr.contains(QJsonValue(colors[i])))
            {
                arr.push_back(QJsonValue(colors[i]));
            }
            if(arr.size() >= 3)
            {
                break;
            }
        }
    }

    QVector<QColor> colors;
    for(int i = 0; i < arr.size(); i++)
    {
        colors.push_back(QColor(arr[i].toString()));
    }

    return { rows, columns, colors };
}

void GameFieldModel::generateBoard()
{
    auto checkPrevious = [this](int row, int column) -> bool {

        if(row > 1)
        {
            if(possibleMatch3(m_gameField[row * m_gameConfig.columns + column],
                              m_gameField[(row - 1) * m_gameConfig.columns + column],
                              m_gameField[(row - 2) * m_gameConfig.columns + column]))
            {
                return true;
            }
        }
        if(column > 1)
        {
            if(possibleMatch3(m_gameField[row * m_gameConfig.columns + column],
                              m_gameField[row * m_gameConfig.columns + (column - 1)],
                              m_gameField[row * m_gameConfig.columns + (column - 2)]))
            {
                return true;
            }
        }
        return false;
    };

    std::srand(time(0));
    do
    {
        for(int i = 0; i < m_gameConfig.rows; i++)
        {
            for(int j = 0; j < m_gameConfig.columns; j++)
            {
                do
                {
                    m_gameField[i * m_gameConfig.columns + j] = Tile(QColor(m_gameConfig.getRandomColor()));
                } while(checkPrevious(i, j));
            }
        }
    } while (!hasMoves());
    m_score = 0;
    emit scoreChanged();
    emit dataChanged(createIndex(0, 0), createIndex(m_gameField.size() - 1, 0));
}

bool GameFieldModel::checkForMatch()
{
    bool wasMatched = false;
    int matches = 0;
    auto tile = m_gameField.begin();
    auto checkNeightbours = [&](int direction, int limit, int shift){
        if(direction > 0 && direction < limit - 1)
        {
            int match = match3(*tile, *(tile + shift), *(tile - shift));
            if(match > 0)
            {
                wasMatched = true;
                matches += match;
            }
        }
    };
    for(int i = 0; i < m_gameConfig.rows; i++)
    {
        for(int j = 0; j < m_gameConfig.columns; j++)
        {
            checkNeightbours(j, m_gameConfig.columns, 1);
            checkNeightbours(i, m_gameConfig.rows, m_gameConfig.columns);
            tile++;
        }
    }

    if(wasMatched)
    {

        if(matches % 3 == 0 && matches != 3)
        {
            m_score += matches + matches / 3 + 1;
        }
        else
        {
            m_score += matches > 3 ? matches + std::round(matches / 3) : matches;
        }

        emit scoreChanged();
    }

    return wasMatched;
}

int GameFieldModel::check(int index)
{
    int row = std::floor(index / m_gameConfig.columns);
    int column = index % m_gameConfig.columns;
    int matches = 0;
    m_gameField[row * m_gameConfig.columns + column].setWasTraversed(true);

    auto checkForMatchLmd = [=, &matches](int direction, int limit, int shift){
        if(direction > 0 && direction < limit - 1)
        {
            int match = match3(m_gameField[row * m_gameConfig.columns + column],
                           m_gameField[row * m_gameConfig.columns + column - shift],
                           m_gameField[row * m_gameConfig.columns + column + shift]);

            if(match > 0)
            {
                matches += match;
            }
        }
    };

    checkForMatchLmd(row, m_gameConfig.rows, m_gameConfig.columns);
    checkForMatchLmd(column, m_gameConfig.columns, 1);

    if(column > 0 && m_gameField[row * m_gameConfig.columns + column] == m_gameField[row * m_gameConfig.columns + column - 1]
            && !m_gameField[row * m_gameConfig.columns + column - 1].wasTraversed())
    {
        matches += check(index - 1);
    }
    if(column < m_gameConfig.columns - 1 && m_gameField[row * m_gameConfig.columns + column] == m_gameField[row * m_gameConfig.columns + column + 1]
            && !m_gameField[row * m_gameConfig.columns + column + 1].wasTraversed())
    {
        matches += check(index + 1);
    }
    if(row > 0 && m_gameField[row * m_gameConfig.columns + column] == m_gameField[row * m_gameConfig.columns + column - m_gameConfig.columns]
            && !m_gameField[row * m_gameConfig.columns + column - m_gameConfig.columns].wasTraversed())
    {
        matches += check(index - m_gameConfig.columns);
    }
    if(row < m_gameConfig.rows - 1 && m_gameField[row * m_gameConfig.columns + column] == m_gameField[row * m_gameConfig.columns + column + m_gameConfig.columns]
            && !m_gameField[row * m_gameConfig.columns + column + m_gameConfig.columns].wasTraversed())
    {
        matches += check(index + m_gameConfig.columns);
    }

    m_gameField[row * m_gameConfig.columns + column].setWasTraversed(false);

    return matches;
}

bool GameFieldModel::checkForMatchSmart(int index)
{
    if(index < 0)
    {
        return false;
    }
    int matches = check(index);
    if(matches > 0)
    {
        if(matches % 3 == 0 && matches != 3)
        {
            m_score += matches + matches / 3 + 1;
        }
        else
        {
            m_score += matches > 3 ? matches + std::round(matches / 3) : matches;
        }
        emit scoreChanged();

        return true;
    }
    return false;
}

int GameFieldModel::match3(Tile& tile1, Tile& tile2, Tile& tile3)
{
    int match = 0;
    auto check = [&match](Tile& tile) {
        if(!tile.isMatched())
        {
            tile.setIsMatched();
            match++;
        }
    };
    if(tile1 == tile2 && tile1 == tile3)
    {
        check(tile1);
        check(tile2);
        check(tile3);
    }
    return match;
}

bool GameFieldModel::possibleMatch3(const Tile &tile1, const Tile &tile2, const Tile &tile3)
{
    return tile1 == tile2 && tile1 == tile3;
}

bool GameFieldModel::hasMoves()
{
    auto tile = m_gameField.begin();
    for(int i = 0; i < m_gameConfig.rows; i++)
    {
        for(int j = 0; j < m_gameConfig.columns; j++)
        {
            if(j < m_gameConfig.columns - 2 && *tile == *(tile + 2))
            {
                if(i > 0 && possibleMatch3(*tile, *(tile + 2), *(tile - m_gameConfig.columns + 1))) return true;
                if(i < m_gameConfig.rows - 1 && possibleMatch3(*tile, *(tile + 2), *(tile + m_gameConfig.columns + 1))) return true;
            }

            if(i < m_gameConfig.rows - 2 && *tile == *(tile + m_gameConfig.columns * 2))
            {
                if(j > 0 && possibleMatch3(*tile, *(tile + m_gameConfig.columns * 2), *(tile + m_gameConfig.columns - 1))) return true;
                if(j < m_gameConfig.columns - 1 && possibleMatch3(*tile, *(tile + m_gameConfig.columns * 2), *(tile + m_gameConfig.columns + 1))) return true;
            }

            if(j < m_gameConfig.columns - 2 && *tile == *(tile + 1))
            {
                if(i > 0 && possibleMatch3(*tile, *(tile + 1), *(tile + 2 - m_gameConfig.columns))) return true;
                if(i < m_gameConfig.rows - 1 && possibleMatch3(*tile, *(tile + 1), *(tile + 2 + m_gameConfig.columns))) return true;
                if(j < m_gameConfig.columns - 3 && possibleMatch3(*tile, *(tile + 1), *(tile + 3))) return true;
            }

            if(j > 1 && *tile == *(tile - 1))
            {
                if(i > 0 && possibleMatch3(*tile, *(tile - 1), *(tile - 2 - m_gameConfig.columns))) return true;
                if(i < m_gameConfig.rows - 1 && possibleMatch3(*tile, *(tile - 1), *(tile - 2 + m_gameConfig.columns))) return true;
                if(j > 2 && possibleMatch3(*tile, *(tile - 1), *(tile - 3))) return true;
            }

            if(i < m_gameConfig.rows - 2 && *tile == *(tile + m_gameConfig.columns))
            {
                if(j > 0 && possibleMatch3(*tile, *(tile + m_gameConfig.columns), *(tile - 1 + 2 * m_gameConfig.columns))) return true;
                if(j < m_gameConfig.columns - 1 && possibleMatch3(*tile, *(tile + m_gameConfig.columns), *(tile + 1 + 2 * m_gameConfig.columns))) return true;
                if(i < m_gameConfig.rows - 3 && possibleMatch3(*tile, *(tile + m_gameConfig.columns), *(tile + 3 * m_gameConfig.columns))) return true;
            }

            if(i > 1 && *tile == *(tile - m_gameConfig.columns))
            {
                if(j > 0 && possibleMatch3(*tile, *(tile - m_gameConfig.columns), *(tile - 1 - 2 * m_gameConfig.columns))) return true;
                if(j < m_gameConfig.columns - 1 && possibleMatch3(*tile, *(tile - m_gameConfig.columns), *(tile + 1 - 2 * m_gameConfig.columns))) return true;
                if(i > 2 && possibleMatch3(*tile, *(tile - m_gameConfig.columns), *(tile - 3 * m_gameConfig.columns))) return true;
            }
            tile++;
        }
    }
    return false;
}

void GameFieldModel::riseDeleted()
{
    for(int i = 0; i < m_gameConfig.rows; i++)
    {
        for(int j = 0; j < m_gameConfig.columns; j++)
        {
            if(m_gameField[i * m_gameConfig.columns + j].isDeleted())
            {
                for(int row = i - 1; row >= 0; row--)
                {
                    moveSwap(row * m_gameConfig.columns + j, (row + 1) * m_gameConfig.columns + j);
                }
            }
        }
    }
}

void GameFieldModel::removeMatched()
{
    for(auto& tile : m_gameField)
    {
        if(tile.isMatched())
        {
            tile.setDeleted();
        }
    }

    emit dataChanged(createIndex(0, 0), createIndex(m_gameField.size(), 0));
}

void GameFieldModel::addNewTiles()
{
    for(int i = 0; i < m_gameField.size(); i++)
    {
        if(m_gameField[i].isDeleted())
        {
            beginRemoveRows(QModelIndex(), i, i);
            m_gameField.remove(i);
            endRemoveRows();

            beginInsertRows(QModelIndex(), i, i);
            m_gameField.insert(i, Tile(m_gameConfig.getRandomColor()));
            endInsertRows();
        }
    }
}

void GameFieldModel::incrementMovesCounter()
{
    m_moves++;
    emit movesChanged();
}

void GameFieldModel::resetMovesCounter()
{
    m_moves = 0;
    emit movesChanged();
}

QHash<int, QByteArray> GameFieldModel::roleNames() const
{
    return m_roleNames;
}

int GameFieldModel::getRows()
{
    return m_gameConfig.rows;
}

int GameFieldModel::getColumns()
{
    return m_gameConfig.columns;
}
