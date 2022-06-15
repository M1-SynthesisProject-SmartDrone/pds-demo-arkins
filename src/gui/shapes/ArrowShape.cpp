#include "ArrowShape.h"

ArrowShape::ArrowShape(const sf::Vector2f& position,
    const sf::Vector2f& headSize,
    const sf::Vector2f& tailSize,
    const sf::Color& color
) : m_head(3), m_tail(tailSize)
{
    m_head.setFillColor(color);
    m_head.setPoint(0, sf::Vector2f(0.f, 0.f));
    m_head.setPoint(1, sf::Vector2f(headSize.x / 2.f, headSize.y));
    m_head.setPoint(2, sf::Vector2f(headSize.x, 0.f));

    // The center is based on if the tail is visible or not
    if (tailSize == sf::Vector2f(0.f, 0.f))
    {
        m_head.setOrigin(sf::Vector2f(headSize.x / 2.f, headSize.y / 2.f));
    }
    else
    {
        m_head.setOrigin(sf::Vector2f(headSize.x / 2.f, 0.f));
        m_tail.setFillColor(color);
        m_tail.setOrigin(sf::Vector2f(tailSize.x / 2.f, tailSize.y));
    }

    setPosition(position);
}

void ArrowShape::setPosition(const sf::Vector2f& position)
{
    m_head.setPosition(position);
    m_tail.setPosition(position);
}

void ArrowShape::setRotation(const float angle)
{
    m_head.setRotation(angle);
    m_tail.setRotation(angle);
}

void ArrowShape::setScale(const sf::Vector2f& scale)
{
    m_head.setScale(scale);
    m_tail.setScale(scale);
}

void ArrowShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_tail);
    target.draw(m_head);
}