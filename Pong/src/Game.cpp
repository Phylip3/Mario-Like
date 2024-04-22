#include "../includes/Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(640, 480), "SFML Pong"), leftPaddle(), rightPaddle() {
    leftPaddle.setSize(sf::Vector2f(10, 100));
    leftPaddle.setPosition(10, 200);
    rightPaddle.setSize(sf::Vector2f(10, 100));
    rightPaddle.setPosition(620, 200);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {

            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {

            handlePlayerInput(event.key.code, true);
        }

        else if (event.type == sf::Event::KeyReleased) {
            handlePlayerInput(event.key.code, false);

        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (isMovingUp == 1) {
        //cout move up
        //std::cout << "up\n";
        leftPaddle.move(0, -200 * deltaTime.asSeconds());
    }
    if (isMovingDown == 1) {
        leftPaddle.move(0, 200 * deltaTime.asSeconds());
    }
    if (isMovingLeft == 1) {
        rightPaddle.move(0, -200 * deltaTime.asSeconds());
    }
    if (isMovingRight == 1) {
        rightPaddle.move(0, 200 * deltaTime.asSeconds());
    }
}

void Game::render() {
    window.clear();
    window.draw(leftPaddle);
    window.draw(rightPaddle);
    window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::Z) {
        std::cout << "Z key pressed\n";
        //count is pressed
        std::cout << isPressed << std::endl;
        isMovingUp = isPressed;
    }
    else if (key == sf::Keyboard::S) {
        std::cout << "S key pressed\n";
        isMovingDown = isPressed;

    }

    else if (key == sf::Keyboard::Up)
    {
        std::cout << "Up key pressed\n";
        isMovingLeft = isPressed;
    }
    else if (key == sf::Keyboard::Down) {
        std::cout << "Down key pressed\n";

        isMovingRight = isPressed;
    }
}
