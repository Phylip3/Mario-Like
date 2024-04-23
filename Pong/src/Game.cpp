#include "../includes/Game.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(640, 480), "SFML Pong"), leftPaddle(10, 200), rightPaddle(620, 200), ball(320, 240)
{
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
    if (isMovingUp) {
        leftPaddle.moveUp(deltaTime);
    }
    if (isMovingDown) {
        leftPaddle.moveDown(deltaTime, window.getSize().y);
    }
    if (isMovingLeft) {
        rightPaddle.moveUp(deltaTime);
    }
    if (isMovingRight) {
        rightPaddle.moveDown(deltaTime, window.getSize().y);
    }

    ball.update(deltaTime, leftPaddle, rightPaddle, window.getSize().y);

}

void Game::render() {
    window.clear();
    leftPaddle.draw(window);
    rightPaddle.draw(window);
    ball.draw(window);
    window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    //if (!isPressed) return;
    
    switch (key) {
    case sf::Keyboard::Space:
        ball.startMovement();
        break;
    case sf::Keyboard::Z:
        isMovingUp = isPressed;
        break;
    case sf::Keyboard::S:
        isMovingDown = isPressed;
        break;
    case sf::Keyboard::Up:
        isMovingLeft = isPressed;
        break;
    case sf::Keyboard::Down:
        isMovingRight = isPressed;
        break;
    }
    
}
