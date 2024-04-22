#include "includes/Game.h"
#include "includes/Server/Server.h" // Include the server's header

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <thread> // Include support for std::thread

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong Menu");
    sf::Font font;

    if (!font.loadFromFile("../../Pong/includes/police/ArialMT.ttf")) {
        std::cerr << "Error loading font\n";
        return 1;
    }

    sf::Text menuItems[2];
    std::string titles[2] = { "Nouveau serveur", "Rejoindre une partie" };
    sf::Text inputText("", font, 24);
    sf::Vector2u windowSize = window.getSize();
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(windowSize.x / 2, windowSize.y / 2);
    sf::Text serverStatus("", font, 24); // Text for displaying server status
    serverStatus.setFillColor(sf::Color::Green);
    serverStatus.setPosition(windowSize.x / 2, windowSize.y / 2); // Position the status message below the menu

    std::string userInput = "";
    bool enteringText = false;
    int selectedItem = -1;
    bool serverRunning = false; // Track if the server is running
    std::thread serverThread; // Thread to handle the server

    for (int i = 0; i < 2; i++) {
        menuItems[i].setFont(font);
        menuItems[i].setString(titles[i]);
        menuItems[i].setCharacterSize(24);
        menuItems[i].setFillColor(sf::Color::White);
        menuItems[i].setPosition(300, 200 + i * 100);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!serverRunning) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (int i = 0; i < 2; i++) {
                            if (menuItems[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                                std::cout << titles[i] << " clicked!\n";
                                enteringText = true;
                                selectedItem = i;
                                if (titles[i] == "Nouveau serveur" && !serverRunning) { // Check if server is not already running
                                    serverRunning = true; // Set server running status
                                    serverThread = std::thread([]() {
                                        startServer(); // Assume mainServer() starts the server
                                        });
                                    serverStatus.setString("Serveur lancé sur le port 2001!"); // Update the status text
                                }
                            }
                        }
                    }
                }
            }
            // Additional input handling here...
        }

        window.clear();
        for (int i = 0; i < 2; i++) {
            window.draw(menuItems[i]);
        }
        inputText.setString(userInput);
        if (enteringText) {
            window.draw(inputText);
        }
        if (serverRunning) {
            window.clear();
            window.draw(serverStatus); // Draw the server status message if the server is running
        }
        window.display();
    }

    if (serverThread.joinable()) {
        serverThread.join(); // Ensure the server thread is properly joined before exiting
    }

    return 0;
}


//int main() {
//    Game game;
//    game.run();
//    return 0;
//}