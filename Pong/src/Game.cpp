#include "../includes/Game.h"

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
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
            handlePlayerInput(event.key.code, true);
        else if (event.type == sf::Event::KeyReleased)
            handlePlayerInput(event.key.code, false);
    }
}

void Game::update(sf::Time deltaTime) {
    if (isMovingUp)
        leftPaddle.move(0, -200 * deltaTime.asSeconds());
    if (isMovingDown)
        leftPaddle.move(0, 200 * deltaTime.asSeconds());
    if (isMovingLeft)
        rightPaddle.move(0, -200 * deltaTime.asSeconds());
    if (isMovingRight)
        rightPaddle.move(0, 200 * deltaTime.asSeconds());
}

void Game::render() {
    window.clear();
    window.draw(leftPaddle);
    window.draw(rightPaddle);
    window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (key == sf::Keyboard::Z)
        isMovingUp = isPressed;
    else if (key == sf::Keyboard::S)
        isMovingDown = isPressed;
    else if (key == sf::Keyboard::Up)
        isMovingLeft = isPressed;
    else if (key == sf::Keyboard::Down)
        isMovingRight = isPressed;
}
