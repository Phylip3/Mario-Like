// Menu.cpp
#include "../../includes/CoreGame/Menu.h"
#include "../../includes/Game.h" // Assuming Game.h has necessary game logic
#include "../../includes/Client/Client.h" // Server functionality
#include "../../includes/Server/Server.h" // Client functionality
#include <iostream>
#include <thread>

Menu::Menu(sf::RenderWindow& win) : window(win), enteringText(false), selectedItem(-1), serverRunning(false), isConnectedFlag(false) {
    if (!font.loadFromFile("../../Pong/includes/police/ArialMT.ttf")) {
        std::cerr << "Error loading font\n";
    }
    titles[0] = "Nouveau serveur";
    titles[1] = "Rejoindre une partie";

    for (int i = 0; i < 2; i++) {
        menuItems[i].setFont(font);
        menuItems[i].setString(titles[i]);
        menuItems[i].setCharacterSize(24);
        menuItems[i].setFillColor(sf::Color::White);
        menuItems[i].setPosition(300, 200 + i * 100);
    }

    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    serverStatus.setFont(font);
    serverStatus.setCharacterSize(24);
    serverStatus.setFillColor(sf::Color::Green);
    serverStatus.setPosition(window.getSize().x / 2, window.getSize().y / 2);
}

void Menu::handleEvent(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        for (int i = 0; i < 2; i++) {
            if (menuItems[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                std::cout << titles[i] << " clicked!\n";
                enteringText = true;
                selectedItem = i;
                if (titles[i] == "Nouveau serveur" && !serverRunning) {
                    serverRunning = true;
                    serverThread = std::thread(startServer);
                    serverStatus.setString("Serveur lancé sur le port 2003!");
                }
                else if (titles[i] == "Rejoindre une partie") {
                    connectToServer();
                    isConnectedFlag = true;
                }
            }
        }
    }
}

void Menu::update() {
    if (serverRunning || isConnectedFlag) {
        window.clear();
    }
}

void Menu::draw() {
    if (!serverRunning && !isConnectedFlag) {
        for (auto& item : menuItems) {
            window.draw(item);
        }
        if (enteringText) {
            inputText.setString(userInput);
            window.draw(inputText);
        }
    }
    if (serverRunning) {
        window.draw(serverStatus);
    }
    if (isConnectedFlag) {
        Game game;
        game.run();
    }
}

bool Menu::isServerRunning() const {
    return serverRunning;
}

bool Menu::isConnected() const {
    return isConnectedFlag;
}
