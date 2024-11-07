
// Node.hpp


// Include guard
#ifndef NODE_H
#define NODE_H

// Include libraries
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Bot.hpp"

// Forward declaration of bot class
class Bot;

// Node class (generic, a junction)
class Node {
    protected:
        // Variables
        sf::Vector2f position;
        std::vector<std::shared_ptr<Node>> connections;
        const float NODE_WIDTH = 20.0f;
        const sf::Vector2f SQUARE_OFFSET = sf::Vector2f(NODE_WIDTH/2, NODE_WIDTH/2);
    public:
        sf::RectangleShape shape;

        // Constructor
        Node(float x, float y);

        // Functions
        sf::Vector2f getPosition() const;
        void setPosition(float x, float y);
        void addConnection(std::shared_ptr<Node> node);
        const std::vector<std::shared_ptr<Node>>& getConnections() const;
};

// Start node class (spawns bots)
class StartNode : public Node {
    public:
        // Constructor
        StartNode(float x, float y);

        // Functions
        std::unique_ptr<Bot> spawnBot();
};

// End node class (no connections, despawns bots)
class EndNode : public Node {
    public:
        // Constructor
        EndNode(float x, float y);
};


#endif
