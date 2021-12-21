#pragma once

#include <QColor>

class Tile
{
public:

    enum class STATE
    {
        NONE,
        DELETED,
        MATCHED
    };

    Tile() = default;
    Tile(const QColor& color);

    QColor getColor() const;
    bool isMatched() const;
    void setIsMatched();
    bool isDeleted() const;
    void setDeleted();
    bool wasTraversed() const;
    void setWasTraversed(bool wasTraversed);

    bool operator==(const Tile& other) const;

private:
    QColor m_color;
    STATE m_state;
    bool m_wasTraversed;
};

