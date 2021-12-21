#include "tile.h"

Tile::Tile(const QColor& color) : m_color(color), m_state(STATE::NONE), m_wasTraversed(false)
{

}

QColor Tile::getColor() const
{
    return m_color;
}

bool Tile::isMatched() const
{
    return m_state == STATE::MATCHED;
}

void Tile::setIsMatched()
{
    m_state = STATE::MATCHED;
}

bool Tile::isDeleted() const
{
    return m_state == STATE::DELETED;
}

void Tile::setDeleted()
{
    m_state = STATE::DELETED;
}

bool Tile::wasTraversed() const
{
    return m_wasTraversed;
}

void Tile::setWasTraversed(bool wasTraversed)
{
    m_wasTraversed = wasTraversed;
}

bool Tile::operator==(const Tile &other) const
{
    return m_color == other.m_color;
}
