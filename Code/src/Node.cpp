
// Node.cpp


// Include libraries
#include <memory>

// Include headers
#include "Node.hpp"
#include "Bot.hpp"


// Node class constructor
Node::Node(float x, float y) : position(x, y), shape(sf::Vector2f(NODE_WIDTH, NODE_WIDTH)) {
    shape.setFillColor(sf::Color::Blue);
    shape.setPosition(position - SQUARE_OFFSET);
}

// Get position of node
sf::Vector2f Node::getPosition() const {
    return position;
}

// Set position of node
void Node::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
    shape.setPosition(position - SQUARE_OFFSET);
}

// Add connection to node
void Node::addConnection(std::shared_ptr<Node> node) {
    connections.push_back(node);
}

// Get connections of node
const std::vector<std::shared_ptr<Node>>& Node::getConnections() const {
    return connections;
}


// Start node class constructor
StartNode::StartNode(float x, float y) : Node(x, y) {
    shape.setFillColor(sf::Color::Green);
}

// Spawn bot at start node
std::unique_ptr<Bot> StartNode::spawnBot() {
    return std::unique_ptr<Bot>(new Bot(position.x, position.y, connections[0].get()));
}


// End node class constructor
EndNode::EndNode(float x, float y) : Node(x, y) {
    shape.setFillColor(sf::Color::Red);
}
