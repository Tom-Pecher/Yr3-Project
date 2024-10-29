
// SMART TRAFFIC LIGHT SYSTEM
// Enivronment Prototype

// Include Libraries
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// Window properties
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Bot properties
const float CIRCLE_RADIUS = 5.0f;
const float SPEED = 100.0f; // pixels per second
const sf::Vector2f CIRCLE_OFFSET(CIRCLE_RADIUS, CIRCLE_RADIUS);

// Start node properties
const float SPAWN_INTERVAL = 0.5f; // seconds

// Default node
class Node {
protected:
    const float NODE_WIDTH = 20.0f;
    const sf::Vector2f NODE_OFFSET = sf::Vector2f(NODE_WIDTH/2, NODE_WIDTH/2);
    sf::RectangleShape shape;
    sf::Vector2f position;
public:
    Node(float x, float y) : position(x, y) {
        shape = sf::RectangleShape(sf::Vector2f(NODE_WIDTH, NODE_WIDTH));
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(position - NODE_OFFSET);
    }

    sf::Vector2f getPosition() const {
        return position;
    }

    void setPosition(float x, float y) {
        position = sf::Vector2f(x, y);
        shape.setPosition(position - NODE_OFFSET);
    }

    sf::RectangleShape getShape() {
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

struct MovingCircle {
    sf::CircleShape shape;
    float distanceTravelled;
};

class Bot {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    float distanceTravelled;
public:
    Bot(float x, float y) : position(x, y) {
        shape = sf::CircleShape(CIRCLE_RADIUS);
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position - CIRCLE_OFFSET);
        distanceTravelled = 0.0f;
    }
};


int main() {
    // Create the SFML window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Network Simulation");

    // Define start and end nodes
    StartNode startNode = StartNode(100.0f, WINDOW_HEIGHT/2);
    EndNode endNode = EndNode(WINDOW_WIDTH - 100.0f, WINDOW_HEIGHT/2);


    // Line between nodes
    sf::Vertex line[] = {
        sf::Vertex(startNode.getPosition()),
        sf::Vertex(endNode.getPosition())
    };

    // Vector to store moving circles
    std::vector<MovingCircle> circles;
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
            MovingCircle newCircle;
            newCircle.shape = sf::CircleShape(CIRCLE_RADIUS);
            newCircle.shape.setFillColor(sf::Color::White);
            newCircle.shape.setPosition(startNode.getPosition() - CIRCLE_OFFSET);
            newCircle.distanceTravelled = 0.0f;
            circles.push_back(newCircle);
        }

        // Calculate the direction vector and length
        sf::Vector2f direction = endNode.getPosition() - startNode.getPosition();
        float lineLength = std::sqrt(direction.x*direction.x + direction.y*direction.y);
        sf::Vector2f normalizedDirection = direction/lineLength;

        // Move each circle along the line
        for (auto it = circles.begin(); it != circles.end();) {
            it->distanceTravelled += SPEED*deltaTime;
            if (it->distanceTravelled >= lineLength) {
                // Remove circle when it reaches the end node
                it = circles.erase(it);
            } else {
                // Update position along the line
                it->shape.setPosition(startNode.getPosition() + normalizedDirection*it->distanceTravelled - CIRCLE_OFFSET);
                ++it;
            }
        }

        // Render everything
        window.clear();
        window.draw(line, 2, sf::Lines);
        for (const auto& circle : circles) {
            window.draw(circle.shape);
            window.draw(startNode.getShape());
            window.draw(endNode.getShape());
        }
        window.display();
    }

    return 0;
}
