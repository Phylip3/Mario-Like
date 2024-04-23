// Ball.cpp
#define _USE_MATH_DEFINES
#include "../../includes/CoreGame/Ball.h"
//include math
#include <iostream>
#include <math.h>
#include <cstdlib>  // Pour rand()
#include <ctime>    // Pour time()

Ball::Ball(float mX, float mY) : velocity(400.f, 400.f) {
    srand(time(NULL));
    ballShape.setRadius(10.f);
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
    // Utilisation de tan pour simuler un angle de rebond qui change progressivement
    float angleRadians = std::atan(impactFactor); // atan retourne un angle en radians
    float newYVelocity = 300 * std::sin(angleRadians); // Utiliser sin pour obtenir la composante y

    std::cout << "New Y velocity: " << newYVelocity << ", Impact factor: " << impactFactor << std::endl;

    // Inversion de la direction horizontale avec une légère augmentation de la vitesse
    float speedIncrease = 1.10; // Augmenter de 10%
    velocity.x = -velocity.x * speedIncrease;
    velocity.y = newYVelocity * speedIncrease;
}

void Ball::startMovement() {
    if (isMoving)
    {
        return;
    }
    // Définir les plages d'angles acceptables (en degrés)
    std::vector<std::pair<int, int>> ranges = {
        {10, 80}, {100, 260}, {280, 350}  // Exclure les plages proches de 90° et 270°
    };

    // Choisir une plage aléatoirement
    int rangeIndex = rand() % ranges.size();
    int angle = rand() % (ranges[rangeIndex].second - ranges[rangeIndex].first) + ranges[rangeIndex].first;

    // Convertir l'angle en radians
    float rad = angle * (M_PI / 180.0f);

    // Définir la vitesse en utilisant l'angle
    velocity.x = 300.0f * cos(rad);
    velocity.y = 300.0f * sin(rad);

    isMoving = true;  // Permettre le mouvement de la balle 
    
}

void Ball::resetPosition(float mX, float mY) {
    ballShape.setPosition(mX, mY);
    isMoving = false;
}

sf::Vector2f Ball::getPosition() const {
    return ballShape.getPosition();
}

float Ball::getRadius() const {
    return ballShape.getRadius();
}
