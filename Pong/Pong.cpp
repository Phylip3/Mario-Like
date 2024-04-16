#include "includes/Game.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <SFML/Window.hpp>

int main() {
    Game game;
    game.run();
    return 0;
}


//int main() {
//    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong Menu");
//    sf::Font font;
//   
//    if (!font.loadFromFile("../../../Pong/includes/police/ArialMT.ttf")) {
//        std::cerr << "Error loading font\n";
//        return 1;
//    }
//
//    sf::Text menuItems[2];
//    std::string titles[2] = { "Nouveau serveur", "Rejoindre une partie" };
//    sf::Text inputText("", font, 24);
//    inputText.setFillColor(sf::Color::White);
//    inputText.setPosition(300, 350);
//    std::string userInput = "";
//    bool enteringText = false;
//    int selectedItem = -1;
//
//    for (int i = 0; i < 2; i++) {
//        menuItems[i].setFont(font);
//        menuItems[i].setString(titles[i]);
//        menuItems[i].setCharacterSize(24);
//        menuItems[i].setFillColor(sf::Color::White);
//        menuItems[i].setPosition(300, 200 + i * 100);
//    }
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//
//            if (event.type == sf::Event::MouseButtonPressed) {
//                if (event.mouseButton.button == sf::Mouse::Left) {
//                    for (int i = 0; i < 2; i++) {
//                        if (menuItems[i].getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
//                            std::cout << titles[i] << " clicked!\n";
//                            enteringText = true;
//                            selectedItem = i;
//                        }
//                    }
//                }
//            }
//
//            if (enteringText && event.type == sf::Event::TextEntered) {
//                if (event.text.unicode == '\b') {  // Handle backspace
//                    if (!userInput.empty()) {
//                        userInput.pop_back();
//                    }
//                }
//                else if (event.text.unicode < 128 && event.text.unicode != '\r' && event.text.unicode != '\n') {
//                    userInput += static_cast<char>(event.text.unicode);
//                }
//            }
//
//            if (enteringText && event.type == sf::Event::KeyPressed) {
//                if (event.key.code == sf::Keyboard::Enter) {
//                    std::cout << "Port entered: " << userInput << std::endl;
//                    if (selectedItem == 0) {
//                        // Code to start server on port 2003
//                        std::cout << "Starting server on port 2003..." << std::endl;
//                    }
//                    else if (selectedItem == 1 && userInput == "2001") {
//                        // Code to try and connect to server on port 2001
//                        std::cout << "Trying to join server on port 2001..." << std::endl;
//                    }
//                    userInput.clear();
//                    enteringText = false;
//                    selectedItem = -1;
//                }
//            }
//        }
//
//        window.clear();
//        for (int i = 0; i < 2; i++) {
//            window.draw(menuItems[i]);
//        }
//        inputText.setString(userInput);
//        if (enteringText) {
//            window.draw(inputText);
//        }
//        window.display();
//    }
//
//    return 0;
//}