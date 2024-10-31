
// Bot.cpp
#include "Bot.hpp"

Bot::Bot(float x, float y, sf::Vector2f target_pos) : position(x, y), target(target_pos), shape(CIRCLE_RADIUS) {
    shape.setFillColor(sf::Color::White);
    shape.setPosition(position - CIRCLE_OFFSET);
}

sf::Vector2f Bot::getPosition() const {
    return position;
}

void Bot::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    shape.setPosition(position - CIRCLE_OFFSET);
}

void Bot::move() {
    sf::Vector2f direction = target - position;
    if (std::hypot(direction.x, direction.y) > 0.1f) {
        direction /= std::hypot(direction.x, direction.y);
        setPosition(position.x + direction.x * SPEED, position.y + direction.y * SPEED);
    }
}
