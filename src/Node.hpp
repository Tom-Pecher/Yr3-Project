
#ifndef NODE_H
#define NODE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Bot.hpp"


class Bot;

class Node {
protected:
    sf::Vector2f position;
    std::vector<std::shared_ptr<Node>> connections;
    const float NODE_WIDTH = 20.0f;
    const sf::Vector2f SQUARE_OFFSET = sf::Vector2f(NODE_WIDTH / 2, NODE_WIDTH / 2);
public:
    sf::RectangleShape shape;

    Node(float x, float y);
    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);
    void addConnection(std::shared_ptr<Node> node);
    const std::vector<std::shared_ptr<Node>>& getConnections() const;
};

class StartNode : public Node {
public:
    StartNode(float x, float y);
    Bot spawnBot();
};

class EndNode : public Node {
public:
    EndNode(float x, float y);
};


#endif
