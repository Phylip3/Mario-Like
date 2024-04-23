#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "CoreGame/Paddle.h"
#include "CoreGame/Ball.h"
#include "CoreGame/ScoreManager.h"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    sf::RenderWindow window;
    Ball ball;

    Paddle leftPaddle;
    Paddle rightPaddle;

    bool isMovingUp = false;
    bool isMovingDown = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;

    ScoreManager scoreManager;
};  

#endif

