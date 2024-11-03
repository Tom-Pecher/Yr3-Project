
// Bot.cpp
#include "Bot.hpp"
#include <typeinfo>
#include <random>


std::random_device rd;
std::mt19937 gen(rd());


Bot::Bot(float x, float y, Node* target_node) : position(x, y), target(target_node) {
    shape = sf::CircleShape(CIRCLE_RADIUS);
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

bool Bot::move(const std::vector<std::unique_ptr<Bot>>& bots) {

    sf::Vector2f direction = target->getPosition() - position;
    if (std::hypot(direction.x, direction.y) > 0.1f) {
        direction /= std::hypot(direction.x, direction.y);

        // for (auto it = bots.begin(); it != bots.end();) {
        //     if (std::hypot((*it)->getPosition().x - (position.x + direction.x * SPEED), (*it)->getPosition().y - (position.y + direction.y * SPEED)) < 3 * CIRCLE_RADIUS && it != *this) {
        //         return false;
        //     }
        //     it++;
        // }

        setPosition(position.x + direction.x * SPEED, position.y + direction.y * SPEED);
    } else {
        if (target->getConnections().size() > 0) {
            changeTarget();
        } else {
            return true;
        }
    }
    return false;
}

void Bot::changeTarget() {
    std::uniform_int_distribution<> distr(0, target->getConnections().size() - 1);
    target = target->getConnections()[distr(gen)].get();
}

Bot::~Bot() {
    sf::Clock clock;
    while (clock.restart().asSeconds() >= 1) {}
}
