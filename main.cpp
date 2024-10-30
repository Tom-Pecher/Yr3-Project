
// SMART TRAFFIC LIGHT SYSTEM
// Enivronment Prototype
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// Window properties
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Start node properties
const float SPAWN_INTERVAL = 0.5f; // seconds

// Default node
class Node {
protected:
    const float NODE_WIDTH = 20.0f;
    const sf::Vector2f SQUARE_OFFSET = sf::Vector2f(NODE_WIDTH / 2, NODE_WIDTH / 2);
    sf::RectangleShape shape;
    sf::Vector2f position;
public:
    Node(float x, float y) : position(x, y) {
        shape = sf::RectangleShape(sf::Vector2f(NODE_WIDTH, NODE_WIDTH));
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

    sf::RectangleShape getShape() const {
        return shape;
    }
};

// Start node
class StartNode : public Node {
public:
    StartNode(float x, float y) : Node(x, y) {
        shape.setFillColor(sf::Color::Green);
    }
};

// End node
class EndNode : public Node {
public:
    EndNode(float x, float y) : Node(x, y) {
        shape.setFillColor(sf::Color::Red);
    }
};

class Bot {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    float CIRCLE_RADIUS = 5.0f;
    sf::Vector2f CIRCLE_OFFSET = sf::Vector2f(CIRCLE_RADIUS, CIRCLE_RADIUS);
    float SPEED = 100.0f;
public:
    Bot(float x, float y) : position(x, y) {
        shape = sf::CircleShape(CIRCLE_RADIUS);
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

    sf::CircleShape getShape() const {
        return shape;
    }

    void move(sf::Vector2f direction, float deltaTime) {
        setPosition(getPosition().x + direction.x * SPEED * deltaTime,
        getPosition().y + direction.y * SPEED * deltaTime);
    }
};

int main() {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Network Simulation");

    // Define start and end nodes
    StartNode startNode = StartNode(100.0f, WINDOW_HEIGHT / 2);
    EndNode endNode = EndNode(WINDOW_WIDTH - 100.0f, WINDOW_HEIGHT / 2);

    // Line between nodes
    sf::Vertex line[] = {
        sf::Vertex(startNode.getPosition()),
        sf::Vertex(endNode.getPosition())
    };

    // Vector to store moving circles
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
            Bot newBot = Bot(startNode.getPosition().x, startNode.getPosition().y);
            bots.push_back(newBot);
        }

        // Calculate the direction vector and length
        sf::Vector2f direction = endNode.getPosition() - startNode.getPosition();
        float lineLength = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f normalizedDirection = direction / lineLength;

        // Move each bot along the line
        for (auto it = bots.begin(); it != bots.end();) {
            it->move(normalizedDirection, deltaTime);
            // Remove the circle if it reaches the end node
            if (it->getPosition().x > endNode.getPosition().x) {
                it = bots.erase(it);
            } else {
                ++it;
            }
        }

        // Render everything
        window.clear();
        window.draw(line, 2, sf::Lines);
        for (const auto& bot : bots) {
            window.draw(bot.getShape());
        }
        window.draw(startNode.getShape());
        window.draw(endNode.getShape());
        window.display();
    }

    return 0;
}
