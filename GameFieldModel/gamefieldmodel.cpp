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


QColor GameConfig::getRandomColor()
{
    return colors[rand() % colors.size()];
}

GameFieldModel::GameFieldModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames[ColorRole] = "name";
    m_roleNames[MatchRole] = "match";

    m_gameConfig = loadGameFieldConfiguration();
    m_gameField.resize(m_gameConfig.rows * m_gameConfig.columns);
    std::srand(time(0));
    for(int i = 0; i < m_gameField.size(); i++)
    {
        m_gameField[i] = Tile(QColor(m_gameConfig.getRandomColor()));
    }

    checkForMatch();
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
    case MatchRole:
        return m_gameField[row].isMatched();
    }

    return QVariant();
}

void GameFieldModel::swap(int from, int to)
{
    if((std::abs(from - to) == 1 && std::floor(from / m_gameConfig.rows) == std::floor(to / m_gameConfig.rows))
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
    }
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

    QVector<QColor> colors;
    for(int i = 0; i < arr.size(); i++)
    {
        colors.push_back(QColor(arr[i].toString()));
    }

    return { rows, columns, colors };
}

void GameFieldModel::checkForMatch()
{
    auto tile = m_gameField.begin();
    for(int i = 0; i < m_gameConfig.rows; i++)
    {
        for(int j = 0; j < m_gameConfig.columns; j++)
        {
            if(j > 0 && j < m_gameConfig.columns - 1)
            {
                match3(*tile, *(tile + 1), *(tile - 1));
            }
            if(i > 0 && i < m_gameConfig.rows - 1)
            {
                match3(*tile, *(tile + m_gameConfig.columns), *(tile - m_gameConfig.columns));
            }

            tile++;
        }
    }
}

void GameFieldModel::match3(Tile& tile1, Tile& tile2, Tile& tile3)
{
    if(tile1 == tile2 && tile1 == tile3)
    {
        tile1.setIsMatched();
        tile2.setIsMatched();
        tile3.setIsMatched();
    }
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
