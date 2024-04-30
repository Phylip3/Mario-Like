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

    int numberOfMenuItems = 2;  
    int spacing = 100; 
    
    float totalWidth = 0;
    std::vector<sf::FloatRect> textRects(numberOfMenuItems);
    for (int i = 0; i < numberOfMenuItems; i++) {
        menuItems[i].setFont(font);
        menuItems[i].setString(titles[i]);
        menuItems[i].setCharacterSize(24);
        menuItems[i].setFillColor(sf::Color::White);

        textRects[i] = menuItems[i].getLocalBounds();
        totalWidth += textRects[i].width;
    }

    // Ajouter les espaces entre les textes au totalWidth
    totalWidth += (numberOfMenuItems - 1) * spacing;

    // Positionner le premier élément de texte
    float currentX = (window.getSize().x - totalWidth) / 2.0f;  // Début du premier texte
    for (int i = 0; i < numberOfMenuItems; i++) {
        menuItems[i].setOrigin(textRects[i].left + textRects[i].width / 2.0f, textRects[i].top + textRects[i].height / 2.0f);
        menuItems[i].setPosition(sf::Vector2f(currentX + textRects[i].width / 2.0f, window.getSize().y / 2.0f));

        // Mise à jour de currentX pour le prochain élément de texte
        currentX += textRects[i].width + spacing;
    }


    inputText.setFont(font);
    inputText.setCharacterSize(24);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    
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

                    serverStatus.setFont(font);
                    serverStatus.setCharacterSize(24);
                    serverStatus.setFillColor(sf::Color::Green);
                    sf::FloatRect textRect = serverStatus.getLocalBounds();
                    serverStatus.setOrigin(textRect.width / 2, textRect.height / 2);
                    serverStatus.setOrigin(serverStatus.getLocalBounds().width / 2, serverStatus.getLocalBounds().height / 2);
                    serverStatus.setPosition(window.getSize().x / 2, window.getSize().y / 2);
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
        window.close();
        Game game;
        game.run();
        window.close();
    }
}

bool Menu::isServerRunning() const {
    return serverRunning;
}

bool Menu::isConnected() const {
    return isConnectedFlag;
}

void Menu::setNotConnected() {
    isConnectedFlag = false;
}
