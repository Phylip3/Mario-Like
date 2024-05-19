#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <thread>
#include "CoreGame/Paddle.h"
#include "CoreGame/Ball.h"
#include "CoreGame/ScoreManager.h"
#include "CoreGame/Menu.h"

class Game {
public:
    Game();
    void run();
    void processServerCommand(char command[1024]);
    void receiveMessages();

    static Game* GetInstance()
    {
        return instance;
    }
private:

    char messageFS;
    static Game* instance;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

    sf::RenderWindow window;
    Ball ball;
    //std::thread serverThread;
    Paddle leftPaddle;
    Paddle rightPaddle;

    sf::Text instructionText;
    sf::Text player1ControlText;
    sf::Text player2ControlText;
    sf::Font font;

    bool isMovingUp = false;
    bool isMovingDown = false;
    bool isMovingUpArrow = false;
    bool isMovingDownArrow = false;

    ScoreManager scoreManager;
};  

#endif

