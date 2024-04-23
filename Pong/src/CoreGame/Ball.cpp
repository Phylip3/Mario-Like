// Ball.cpp
#define _USE_MATH_DEFINES
#include "../../includes/CoreGame/Ball.h"
//include math
#include <iostream>
#include <math.h>
#include <cstdlib>  // Pour rand()
#include <ctime>    // Pour time()

Ball::Ball(float mX, float mY) : velocity(200.f, 200.f) {
    srand(time(NULL));
    ballShape.setRadius(30.f);
    //ballShape.setFillColor(sf::Color::Green);
    ballShape.setPosition(mX, mY);
}

// Ball.cpp
void Ball::update(sf::Time deltaTime, const Paddle& leftPaddle, const Paddle& rightPaddle, float windowHeight) {
    // Déplacement de la balle
    if (!isMoving) return;// Ne pas bouger si le jeu n'a pas commencé
   

    ballShape.move(velocity * deltaTime.asSeconds());

    // Collision avec le haut ou le bas de l'écran
    sf::Vector2f position = ballShape.getPosition();
    float radius = ballShape.getRadius();
    if (position.y <= 0 || position.y + 2 * radius >= windowHeight) {
        velocity.y = -velocity.y;
        ballShape.setPosition(position.x, position.y < 0 ? 0 : windowHeight - 2 * radius);
    }

    // Collision avec les raquettes
    if (checkCollisionWithPaddle(leftPaddle)) {
        bounceOffPaddle(leftPaddle);
    }
    if (checkCollisionWithPaddle(rightPaddle)) {
        bounceOffPaddle(rightPaddle);
    }
        
    
}



void Ball::draw(sf::RenderWindow &window) {
    window.draw(ballShape);
}

void Ball::setDirection(float x, float y) {
    velocity = sf::Vector2f(x, y);
}

// Ball.cpp

bool Ball::checkCollisionWithPaddle(const Paddle& paddle) {
    sf::Vector2f ballCenter = ballShape.getPosition() + sf::Vector2f(ballShape.getRadius(), ballShape.getRadius());
    sf::FloatRect paddleBounds = paddle.getGlobalBounds();
    sf::Vector2f nearestPoint;

    nearestPoint.x = std::max(paddleBounds.left, std::min(ballCenter.x, paddleBounds.left + paddleBounds.width));
    nearestPoint.y = std::max(paddleBounds.top, std::min(ballCenter.y, paddleBounds.top + paddleBounds.height));

    float dx = nearestPoint.x - ballCenter.x;
    float dy = nearestPoint.y - ballCenter.y;

    return (dx * dx + dy * dy) <= (ballShape.getRadius() * ballShape.getRadius());
}

void Ball::bounceOffPaddle(const Paddle& paddle) {
    float paddleCenter = paddle.getPosition().y + paddle.getSize().y / 2;
    float ballCenter = ballShape.getPosition().y + ballShape.getRadius();
    float difference = ballCenter - paddleCenter;
    float impactFactor = difference / (paddle.getSize().y / 2); // -1 à 1 où 0 est le centre

    // Influence verticale basée sur le point d'impact
    float newYVelocity = 200 * impactFactor; // Multiplier par 5 pour augmenter l'effet

    // Inversion de la direction horizontale
    float speedIncrease = 1.05; // Augmenter de 5%
    velocity.x = -velocity.x * speedIncrease;
    velocity.y = newYVelocity * speedIncrease;
}

void Ball::startMovement() {
    if (isMoving)
    {
        return;
    }
    float angle = (rand() % 360) * (M_PI / 180.0f);  // Angle aléatoire en radians
    std::cout << "Balle lancée!" << (rand() % 360) * (M_PI / 180.0f) << std::endl; 
    velocity.x = 200.0f * std::cos(angle);  // Calculer la composante x basée sur l'angle
    velocity.y = 200.0f * std::sin(angle);  // Calculer la composante y basée sur l'angle
    isMoving = true;  // Permettre le mouvement de la balle
    
}

void Ball::resetPosition(float mX, float mY) {
    ballShape.setPosition(mX, mY);
    isMoving = false;
}
