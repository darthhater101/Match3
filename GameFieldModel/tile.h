#pragma once

#include <QColor>

class Tile
{
public:
    Tile() = default;
    Tile(const QColor& color);

    QColor getColor() const;
    bool isMatched() const;
    void setIsMatched();

    bool operator==(const Tile& other) const;

private:
    QColor m_color;
    bool m_isMatched;
};

