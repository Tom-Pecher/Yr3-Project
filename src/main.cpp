
// SMART TRAFFIC LIGHT SYSTEM
// Environment Prototype
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <memory>
#include "Node.hpp"
#include "Bot.hpp"


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float SPAWN_INTERVAL = 0.5f;


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
        auto startNode = std::make_shared<StartNode>(100.0f, WINDOW_HEIGHT / 2 + 100.0f);
        auto middleNode1 = std::make_shared<Node>(250.0f, WINDOW_HEIGHT / 2 - 100.0f);
        auto middleNode2 = std::make_shared<Node>(WINDOW_WIDTH - 250.0f, WINDOW_HEIGHT / 2 - 100.0f);
        auto endNode = std::make_shared<EndNode>(WINDOW_WIDTH - 100.0f, WINDOW_HEIGHT / 2 + 100.0f);

        // Set up connections
        startNode->addConnection(middleNode1);
        middleNode1->addConnection(middleNode2);
        middleNode2->addConnection(endNode);

        std::vector<std::shared_ptr<Node>> nodes = {startNode, middleNode1, middleNode2, endNode};
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
