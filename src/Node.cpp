
// Node.cpp
#include "Node.hpp"
#include "Bot.hpp"

Node::Node(float x, float y) : position(x, y), shape(sf::Vector2f(NODE_WIDTH, NODE_WIDTH)) {
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(position - SQUARE_OFFSET);
}

sf::Vector2f Node::getPosition() const {
    return position;
}

void Node::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    shape.setPosition(position - SQUARE_OFFSET);
}

void Node::addConnection(std::shared_ptr<Node> node) {
    connections.push_back(node);
}

const std::vector<std::shared_ptr<Node>>& Node::getConnections() const {
    return connections;
}

StartNode::StartNode(float x, float y) : Node(x, y) {
    shape.setFillColor(sf::Color::Green);
}

Bot StartNode::spawnBot() {
    return Bot(position.x, position.y, connections[0]->getPosition());
}

EndNode::EndNode(float x, float y) : Node(x, y) {
    shape.setFillColor(sf::Color::Red);
}
