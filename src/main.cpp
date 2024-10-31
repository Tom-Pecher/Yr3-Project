// SMART TRAFFIC LIGHT SYSTEM
// Environment Prototype
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <memory>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float SPAWN_INTERVAL = 0.5f; // seconds

// Bot (moving vehicle)
class Bot {
private:
    sf::Vector2f position;
    sf::Vector2f target;
    float SPEED = 0.01f;
    float CIRCLE_RADIUS = 5.0f;
    sf::Vector2f CIRCLE_OFFSET = sf::Vector2f(CIRCLE_RADIUS, CIRCLE_RADIUS);
public:
    sf::CircleShape shape;

    Bot(float x, float y, sf::Vector2f target_pos) : position(x, y), target(target_pos) , shape(CIRCLE_RADIUS) {
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position - CIRCLE_OFFSET);
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    void setPosition(float x, float y) {
        position = sf::Vector2f(x, y);
        shape.setPosition(position - CIRCLE_OFFSET);
    }

    void move() {
        sf::Vector2f direction = target - position;
        if (std::hypot(direction.x, direction.y) > 0.1f) { // Prevent overshooting
            direction /= std::hypot(direction.x, direction.y); // Normalize
            setPosition(position.x + direction.x * SPEED, position.y + direction.y * SPEED);
        }
    }
};

// Default node
class Node {
protected:
    sf::Vector2f position;
    std::vector<std::shared_ptr<Node>> connections;
    const float NODE_WIDTH = 20.0f;
    const sf::Vector2f SQUARE_OFFSET = sf::Vector2f(NODE_WIDTH / 2, NODE_WIDTH / 2);
public:
    sf::RectangleShape shape;

    Node(float x, float y) : position(x, y), shape(sf::Vector2f(NODE_WIDTH, NODE_WIDTH)) {
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(position - SQUARE_OFFSET);
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    void setPosition(float x, float y) {
        position = sf::Vector2f(x, y);
        shape.setPosition(position - SQUARE_OFFSET);
    }

    void addConnection(std::shared_ptr<Node> node) {
        connections.push_back(node);
    }

    const std::vector<std::shared_ptr<Node>>& getConnections() const {
        return connections;
    }
};

// Start node
class StartNode : public Node {
public:
    StartNode(float x, float y) : Node(x, y) {
        shape.setFillColor(sf::Color::Green);
    }

    Bot spawnBot() {
        return Bot(position.x, position.y, connections[0]->getPosition());
    }
};

// End node
class EndNode : public Node {
public:
    EndNode(float x, float y) : Node(x, y) {
        shape.setFillColor(sf::Color::Red);
    }
};

// Render everything
void render(const std::vector<Bot>& bots, const std::vector<std::shared_ptr<Node>>& nodes, sf::RenderWindow& window) {
    window.clear();
    for (const auto& bot : bots) {
        window.draw(bot.shape);
    }
    for (const auto& node : nodes) {
        for (const auto& n : node->getConnections()) {
            sf::Vertex line[] = {
                sf::Vertex(node->getPosition(), sf::Color::White),
                sf::Vertex(n->getPosition(), sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }
    }
    for (const auto& node : nodes) {
        window.draw(node->shape);
    }
    window.display();
}

// Simulation environment
class Simulation {
public:
    Simulation() {}

    void main_loop() {
        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Network Simulation");

        // Create nodes using shared_ptr
        auto startNode = std::make_shared<StartNode>(100.0f, WINDOW_HEIGHT / 2);
        auto middleNode = std::make_shared<Node>(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 100.0f);
        auto endNode = std::make_shared<EndNode>(WINDOW_WIDTH - 100.0f, WINDOW_HEIGHT / 2);

        // Set up connections
        startNode->addConnection(middleNode);
        middleNode->addConnection(startNode);
        middleNode->addConnection(endNode);
        endNode->addConnection(middleNode);

        std::vector<std::shared_ptr<Node>> nodes = {startNode, middleNode, endNode};
        std::vector<Bot> bots;
        float spawnTimer = 0.0f;

        // Main loop
        sf::Clock clock;
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            // Update the time
            float deltaTime = clock.restart().asSeconds();
            spawnTimer += deltaTime;

            // Spawn a new circle if the interval has passed
            if (spawnTimer >= SPAWN_INTERVAL) {
                spawnTimer -= SPAWN_INTERVAL;
                Bot newBot = startNode->spawnBot();
                bots.push_back(newBot);
            }

            // Move each bot along the line
            for (auto it = bots.begin(); it != bots.end();) {
                it->move();
                // Remove the circle if it reaches the end node
                if (it->shape.getGlobalBounds().intersects(endNode->shape.getGlobalBounds())) {
                    it = bots.erase(it);
                } else {
                    ++it;
                }
            }

            render(bots, nodes, window);
        }
    }
};

// Main function
int main() {
    Simulation simulation;
    simulation.main_loop();
    return 0;
}
