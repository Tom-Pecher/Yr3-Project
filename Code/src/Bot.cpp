
// Bot.cpp


// Include libraries
#include <random>
#include <typeinfo>

// Include headers
#include "Bot.hpp"


// Global random number generator
std::random_device rd;
std::mt19937 gen(rd());


// Bot class constructor
Bot::Bot(float x, float y, Node* target_node) : position(x, y), target(target_node) {
    shape = sf::CircleShape(CIRCLE_RADIUS);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(position - CIRCLE_OFFSET);
}

// Get position of bot
sf::Vector2f Bot::getPosition() const {
    return position;
}

// Set position of bot
void Bot::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    shape.setPosition(position - CIRCLE_OFFSET);
}

// Move bot along edge
bool Bot::move(const std::vector<std::unique_ptr<Bot>>& bots) {
    sf::Vector2f direction = target->getPosition() - position;
    // If bot is not at target, move towards target
    if (std::hypot(direction.x, direction.y) > 0.1f) {
        direction /= std::hypot(direction.x, direction.y);

        // for (auto it = bots.begin(); it != bots.end();) {
        //     if (std::hypot((*it)->getPosition().x - (position.x + direction.x*SPEED), (*it)->getPosition().y - (position.y + direction.y*SPEED)) < 3*CIRCLE_RADIUS && it != *this) {
        //         return false;
        //     }
        //     it++;
        // }

        setPosition(position.x + direction.x*SPEED, position.y + direction.y*SPEED);
    } else {
        // If bot is at target and target is not the end, change target
        if (target->getConnections().size() > 0) {
            changeTarget();
        // If target is the end, signal to remove bot
        } else {
            return true;
        }
    }
    return false;
}

// Change target to random connection (pick an exit at a junction)
void Bot::changeTarget() {
    std::uniform_int_distribution<> distr(0, target->getConnections().size() - 1);
    target = target->getConnections()[distr(gen)].get();
}

// Bot class destructor
Bot::~Bot() {
}
