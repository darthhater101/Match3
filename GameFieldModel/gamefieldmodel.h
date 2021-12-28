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
    Q_PROPERTY(int score MEMBER m_score NOTIFY scoreChanged)
    Q_PROPERTY(int moves MEMBER m_moves NOTIFY movesChanged)

public:

    enum RoleNames {
        ColorRole = Qt::UserRole,
        DeletedRole = Qt::UserRole + 2,
    };

    explicit GameFieldModel(QObject *parent = nullptr);
    ~GameFieldModel();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    int match3(Tile& tile1, Tile& tile2, Tile& tile3);
    bool possibleMatch3(const Tile& tile1, const Tile& tile2, const Tile& tile3) const;
    int check(int index);

    Q_INVOKABLE bool swap(int from, int to);
    Q_INVOKABLE bool hasMoves() const;
    Q_INVOKABLE void generateBoard();
    Q_INVOKABLE bool checkForMatch();
    Q_INVOKABLE bool moveSwap(int from, int to);
    Q_INVOKABLE void riseDeleted();
    Q_INVOKABLE void removeMatched();
    Q_INVOKABLE void addNewTiles();
    Q_INVOKABLE void incrementMovesCounter();
    Q_INVOKABLE void resetMovesCounter();
    Q_INVOKABLE bool checkForMatchSmart(int index);

protected:
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Tile> m_gameField;
    GameConfig m_gameConfig;
    QHash<int, QByteArray> m_roleNames;

    int m_score;
    int m_moves;

    int getRows() const;
    int getColumns() const;

    GameConfig loadGameFieldConfiguration() const;

signals:
    void scoreChanged();
    void movesChanged();
};
