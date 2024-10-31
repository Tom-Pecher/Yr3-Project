
// Bot.hpp
#ifndef BOT_H
#define BOT_H

#include <SFML/Graphics.hpp>
#include <cmath>

class Bot {
private:
    sf::Vector2f position;
    sf::Vector2f target;
    float SPEED = 0.03f;
    float CIRCLE_RADIUS = 5.0f;
    sf::Vector2f CIRCLE_OFFSET = sf::Vector2f(CIRCLE_RADIUS, CIRCLE_RADIUS);
public:
    sf::CircleShape shape;

    Bot(float x, float y, sf::Vector2f target_pos);

    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    void move();
};

#endif
