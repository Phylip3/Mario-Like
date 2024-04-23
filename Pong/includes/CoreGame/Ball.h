// Ball.h
#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

#include "Paddle.h"

class Ball {
public:
    Ball(float mX, float mY);
    void update(sf::Time deltaTime, const Paddle& leftPaddle, const Paddle& rightPaddle, float windowHeight);
    void draw(sf::RenderWindow &window);
    void bounceOffPaddle(const Paddle& paddle);
    void startMovement();
    void resetPosition(float mX, float mY);
    bool checkCollisionWithPaddle(const Paddle& paddle);
    void setDirection(float x, float y);

private:
    sf::CircleShape ballShape;
    sf::Vector2f velocity;
    float maxBounceAngle = 75.0f; // Maximum reflection angle
    bool isMoving = false;
};

#endif
