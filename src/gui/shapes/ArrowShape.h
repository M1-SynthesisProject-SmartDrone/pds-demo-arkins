#ifndef __ARROWSHAPE_H__
#define __ARROWSHAPE_H__

#include <SFML/Graphics.hpp>

/**
 * SFML doesn't include arrows directly, so we created the class
 * ourselves !
 * 
 * @author Aldric Vitali Silvestre
 */
class ArrowShape : public sf::Drawable
{
private:
    sf::RectangleShape m_tail;
    sf::ConvexShape m_head;    

public:
    /**
     * Create the arrow shape with the arrowhead pointing down. 
     * If the tail length not higher than zero, the arrow
     * will have no tail (and be a simple triangle), and the center 
     * will be set on the center of the head.
     * Otherwise, the origin is the intersection between
     * the head and the tail of the arrow
     */ 
    ArrowShape(const sf::Vector2f& position = sf::Vector2f(0.f, 0.f), 
        const sf::Vector2f& headSize = sf::Vector2f(30.f, 30.f),
        const sf::Vector2f& tailSize = sf::Vector2f(0.f, 0.f),
        const sf::Color& color = sf::Color::Black
    );

    /**
     * Set the position of the arrow (note : the origin is the intersection between
     * the head and the tail of the arrow)
     */
    void setPosition(const sf::Vector2f& position);

    void setRotation(const float angle);

    void setScale(const sf::Vector2f& scale);

    // Method to override from Drawable 
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;
};

#endif // __ARROWSHAPE_H__