#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <QColor>
#include <QHash>

#include "tile.h"

struct GameConfig
{
    int rows;
    int columns;
    QVector<QColor> colors;

    QColor getRandomColor();
};

class GameFieldModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rows READ getRows FINAL CONSTANT)
    Q_PROPERTY(int columns READ getColumns FINAL CONSTANT)

public:

    enum RoleNames {
        ColorRole = Qt::UserRole,
        MatchRole = Qt::UserRole + 2,
        DeletedRole = Qt::UserRole + 3,
        PossibleRole = Qt::UserRole + 4
    };

    explicit GameFieldModel(QObject *parent = nullptr);
    ~GameFieldModel();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;


    bool match3(Tile& tile1, Tile& tile2, Tile& tile3);
    bool possibleMatch3(Tile& tile1, Tile& tile2, Tile& tile3);

    Q_INVOKABLE bool hasMoves();

    Q_INVOKABLE void generateBoard();

    Q_INVOKABLE bool checkForMatch();
    Q_INVOKABLE bool swap(int first, int second);
    Q_INVOKABLE void riseDeleted();
    Q_INVOKABLE void removeMatched();
    Q_INVOKABLE void add();

protected:
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Tile> m_gameField;
    GameConfig m_gameConfig;
    QHash<int, QByteArray> m_roleNames;

    int m_score;

    int getRows();
    int getColumns();

    GameConfig loadGameFieldConfiguration();
};
