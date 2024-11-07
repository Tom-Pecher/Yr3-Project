
// Bot.hpp
#ifndef BOT_H
#define BOT_H


// Include libraries
#include <cmath>
#include <SFML/Graphics.hpp>

// Include headers
#include "Node.hpp"


// Forward declaration of node class
class Node;


// Bot class (vehicles, move along edges)
class Bot {
private:
    // Variables
    sf::Vector2f position;
    Node* target;
    float SPEED = 0.03f;
    float CIRCLE_RADIUS = 5.0f;
    sf::Vector2f CIRCLE_OFFSET = sf::Vector2f(CIRCLE_RADIUS, CIRCLE_RADIUS);
public:
    sf::CircleShape shape;

    // Constructor
    Bot(float x, float y, Node* target_node);

    // Destructor
    ~Bot();

    // Functions
    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    bool move(const std::vector<std::unique_ptr<Bot>>& bots);
    void changeTarget();
};


#endif
