
// Bot.hpp
#ifndef BOT_H
#define BOT_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include "Node.hpp"


class Node;

class Bot {
private:
    sf::Vector2f position;
    Node* target;
    float SPEED = 0.03f;
    float CIRCLE_RADIUS = 5.0f;
    sf::Vector2f CIRCLE_OFFSET = sf::Vector2f(CIRCLE_RADIUS, CIRCLE_RADIUS);
public:
    sf::CircleShape shape;

    Bot(float x, float y, Node* target_node);
    ~Bot();

    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    bool move(std::vector<Bot> bots);
    void changeTarget();
};


#endif
