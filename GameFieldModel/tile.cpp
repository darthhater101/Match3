#include "tile.h"

Tile::Tile(const QColor& color) : m_color(color), m_isMatched(false)
{

}

QColor Tile::getColor() const
{
    return m_color;
}

bool Tile::isMatched() const
{
    return m_isMatched;
}

void Tile::setIsMatched()
{
    m_isMatched = true;
}

bool Tile::operator==(const Tile &other) const
{
    return m_color == other.m_color;
}
