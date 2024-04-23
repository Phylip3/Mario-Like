// Paddle.h
#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle {
public:
    Paddle(float posX, float posY);

    void moveUp(sf::Time deltaTime);
    void moveDown(sf::Time deltaTime, float windowHeight);
    void draw(sf::RenderWindow &window);
    sf::Vector2f getSize() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getGlobalBounds() const;

private:
    sf::RectangleShape paddleShape;
    float velocity;
};

#endif
