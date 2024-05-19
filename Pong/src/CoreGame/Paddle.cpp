#include "../../includes/CoreGame/Paddle.h"

Paddle::Paddle(float mX, float mY) : paddleShape(sf::Vector2f(10, 100)), velocity(200), speed(300) {
    // Positionner la raquette
    paddleShape.setPosition(mX, mY);
    
}

void Paddle::moveUp(sf::Time deltaTime) {
    float move = -speed * deltaTime.asSeconds();
    // Empêcher la raquette de dépasser le haut de la fenêtre
    if (paddleShape.getPosition().y + move > 0) {
        paddleShape.move(0, move);
    }
}

void Paddle::moveDown(sf::Time deltaTime, float windowHeight) {
    float move = speed * deltaTime.asSeconds();
    // Empêcher la raquette de dépasser le bas de la fenêtre
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

// Obtenir les limites globales de la raquette
sf::FloatRect Paddle::getGlobalBounds() const {
    return paddleShape.getGlobalBounds();
}

// Obtenir la position de la raquette
sf::Vector2f Paddle::getPosition() const {
    return paddleShape.getPosition();
}
