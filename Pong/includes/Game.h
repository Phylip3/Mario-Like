#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <thread>
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
    void receiveServerMessages();
    void processServerCommand(const std::string& command);

    sf::RenderWindow window;
    Ball ball;
    std::thread serverThread;
    Paddle leftPaddle;
    Paddle rightPaddle;

    bool isMovingUp = false;
    bool isMovingDown = false;
    bool isMovingLeft = false;
    bool isMovingRight = false;

    ScoreManager scoreManager;
};  

#endif

