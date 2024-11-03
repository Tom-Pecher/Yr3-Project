
// SMART TRAFFIC LIGHT SYSTEM
// Environment Prototype
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <memory>
#include "Node.hpp"
#include "Bot.hpp"


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float SPAWN_INTERVAL = 0.5f;


// Render everything
void render(const std::vector<Bot>& bots, const std::vector<std::shared_ptr<Node>>& nodes, sf::RenderWindow& window, sf::Text fpsText) {
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
    window.draw(fpsText);
    window.display();
}


// Simulation environment
class Simulation {
public:
    Simulation() {}

    void main_loop() {
        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Network Simulation");
        //window.setFramerateLimit(1000);

        sf::Font font;
        if (!font.loadFromFile("src/font/Roboto-Bold.ttf")) {
            std::cerr << "Could not load font!" << std::endl;
        }

        sf::Text fpsText;
        fpsText.setFont(font);
        fpsText.setCharacterSize(30);
        fpsText.setFillColor(sf::Color::White);
        fpsText.setPosition(10.f, 10.f);

        // Create nodes using shared_ptr
        auto startNode1 = std::make_shared<StartNode>(150.0f, 450.0f);
        auto startNode2 = std::make_shared<StartNode>(500.0f, 150.0f);
        auto middleNode1 = std::make_shared<Node>(300.0f, 300.0f);
        auto middleNode2 = std::make_shared<Node>(500.0f, 300.0f);
        auto endNode1 = std::make_shared<EndNode>(300.0f, 150.0f);
        auto endNode2 = std::make_shared<EndNode>(650.0f, 450.0f);

        // Set up connections
        startNode1->addConnection(middleNode1);
        middleNode1->addConnection(middleNode2);
        middleNode1->addConnection(endNode1);
        middleNode2->addConnection(endNode2);
        startNode2->addConnection(middleNode2);

        std::vector<std::shared_ptr<Node>> nodes = {startNode1, startNode2, middleNode1, middleNode2, endNode1, endNode2};
        std::vector<Bot> bots;
        float spawnTimer = 0.0f;

        // Main loop
        sf::Clock clock;
        float fps = 0.0f;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            // Update the time
            float currentTime = clock.restart().asSeconds();
            spawnTimer += currentTime;

            fps = 1.0f / currentTime;
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

            // Spawn a new circle if the interval has passed
            if (spawnTimer >= SPAWN_INTERVAL) {
                spawnTimer -= SPAWN_INTERVAL;
                Bot newBot1 = startNode1->spawnBot();
                Bot newBot2 = startNode2->spawnBot();
                bots.push_back(newBot1);
                bots.push_back(newBot2);
            }

            // Move each bot along the line
            for (auto it = bots.begin(); it != bots.end();) {
                if (it->move(bots)) {
                    bots.erase(it);
                } else {
                    ++it;
                }
            }

            render(bots, nodes, window, fpsText);
        }
    }
};


// Main function
int main() {
    Simulation simulation;
    simulation.main_loop();
    return 0;
}
