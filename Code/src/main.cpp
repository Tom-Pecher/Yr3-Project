
// SMART TRAFFIC LIGHT SYSTEM
// by Tom Pecher


// Include libraries
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

// Include headers
#include "Bot.hpp"
#include "Node.hpp"

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float SPAWN_INTERVAL = 0.3f;


// Render everything
void render(const std::vector<std::unique_ptr<Bot>>& bots, const std::vector<std::shared_ptr<Node>>& nodes, sf::RenderWindow& window, sf::Text fpsText) {
    window.clear();

    // Draw all edges
    for (const auto& node : nodes) {
        for (const auto& n : node->getConnections()) {
            sf::Vertex line[] = {
                sf::Vertex(node->getPosition(), sf::Color::White),
                sf::Vertex(n->getPosition(), sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }
    }

    // Draw all nodes
    for (const auto& node : nodes) {
        window.draw(node->shape);
    }

    // Draw all bots
    for (const auto& bot : bots) {
        window.draw(bot->shape);

    }
    // Draw FPS
    window.draw(fpsText);

    window.display();
}


// Simulation class
class Simulation {
public:
    Simulation() {
        main_loop();
    }

    // Main logic loop
    void main_loop() {
        // Create window
        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Network Simulation");
        // window.setFramerateLimit(10000);

        // Load font for FPS text
        sf::Font font;
        if (!font.loadFromFile("src/font/Roboto-Bold.ttf")) {
            std::cerr << "Could not load font!" << std::endl;
        }
        sf::Text fpsText;
        fpsText.setFont(font);
        fpsText.setCharacterSize(30);
        fpsText.setFillColor(sf::Color::White);
        fpsText.setPosition(10.f, 10.f);

        // Create nodes
        auto startNode1 = std::make_shared<StartNode>(150.0f, 450.0f);
        auto startNode2 = std::make_shared<StartNode>(500.0f, 150.0f);
        auto middleNode1 = std::make_shared<Node>(300.0f, 300.0f);
        auto middleNode2 = std::make_shared<Node>(500.0f, 300.0f);
        auto middleNode3 = std::make_shared<Node>(400.0f, 450.0f);
        auto endNode1 = std::make_shared<EndNode>(300.0f, 150.0f);
        auto endNode2 = std::make_shared<EndNode>(650.0f, 450.0f);

        // Create connections
        startNode1->addConnection(middleNode1);
        middleNode1->addConnection(middleNode2);
        middleNode1->addConnection(endNode1);
        middleNode2->addConnection(endNode2);
        middleNode2->addConnection(middleNode3);
        middleNode3->addConnection(middleNode1);
        startNode2->addConnection(middleNode2);

        // Store nodes and bots
        std::vector<std::shared_ptr<Node>> nodes = {startNode1, startNode2, middleNode1, middleNode2, middleNode3, endNode1, endNode2};
        std::vector<std::unique_ptr<Bot>> bots;

        // Create clock and related variables
        sf::Clock clock;
        float spawnTimer = 0.0f;
        float fps = 0.0f;

        // Main loop
        while (window.isOpen()) {
            // Handle events until window is closed
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            // Update spawn timer
            float currentTime = clock.restart().asSeconds();
            spawnTimer += currentTime;

            // Calculate FPS
            fps = 1.0f/currentTime;
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

            // Spawn bots at intervals
            if (spawnTimer >= SPAWN_INTERVAL) {
                spawnTimer -= SPAWN_INTERVAL;
                bots.push_back(std::make_unique<Bot>(startNode1->getPosition().x, startNode1->getPosition().y, startNode1->getConnections()[0].get()));
                bots.push_back(std::make_unique<Bot>(startNode2->getPosition().x, startNode2->getPosition().y, startNode2->getConnections()[0].get()));
            }

            // Move each bot along the line
            for (auto it = bots.begin(); it != bots.end();) {
                if ((*it)->move(bots)) {
                    it = bots.erase(it);
                } else {
                    ++it;
                }
            }

            // Render everything
            render(bots, nodes, window, fpsText);
        }
    }
};


// Main function
int main() {
    Simulation simulation;

    return 0;
}
