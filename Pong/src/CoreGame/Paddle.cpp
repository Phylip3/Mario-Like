#include "../../includes/CoreGame/Paddle.h"

Paddle::Paddle(float mX, float mY) : paddleShape(sf::Vector2f(10, 100)), velocity(200), speed(300) {
    paddleShape.setPosition(mX, mY);
    
}

void Paddle::moveUp(sf::Time deltaTime) {
    float move = -speed * deltaTime.asSeconds();
    if (paddleShape.getPosition().y + move > 0) {
        paddleShape.move(0, move);
    }
}

void Paddle::moveDown(sf::Time deltaTime, float windowHeight) {
    float move = speed * deltaTime.asSeconds();
    if (paddleShape.getPosition().y + paddleShape.getSize().y + move < windowHeight) {
        paddleShape.move(0, move);
    }
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(paddleShape);
}

sf::Vector2f Paddle::getSize() const {
    return paddleShape.getSize();
}

// Paddle.cpp
sf::FloatRect Paddle::getGlobalBounds() const {
    return paddleShape.getGlobalBounds();
}
sf::Vector2f Paddle::getPosition() const {
    return paddleShape.getPosition();
}
