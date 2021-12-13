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
            MatchRole = Qt::UserRole + 2
        };

    explicit GameFieldModel(QObject *parent = nullptr);
    ~GameFieldModel();

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE void swap(int first, int second);

    void checkForMatch();
    void match3(Tile& tile1, Tile& tile2, Tile& tile3);

protected:
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Tile> m_gameField;
    GameConfig m_gameConfig;
    QHash<int, QByteArray> m_roleNames;

    int getRows();
    int getColumns();

    GameConfig loadGameFieldConfiguration();
};
