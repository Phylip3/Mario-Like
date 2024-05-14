#include "../includes/Game.h"
#include "../includes/Client/Client.h"
#include "../includes/CoreGame/Menu.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SFML/Network.hpp>
#include <stdexcept>

Game* Game::instance = nullptr;

Game::Game() : window(sf::VideoMode(640, 480), "SFML Pong"), leftPaddle( 10 , (window.getSize().y/2) - 50), rightPaddle(window.getSize().x - 20, window.getSize().y/2 - 50), ball(window.getSize().x/2, window.getSize().y/2), scoreManager(window)
{
    if (instance == nullptr)
    {
        instance = this;
    }

    if (!font.loadFromFile("../../Pong/includes/police/ArialMT.ttf")) {
        std::cout << "Error loading font\n";
    }

    instructionText.setFont(font);
    instructionText.setString("Press space to start");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    sf::FloatRect textRect = instructionText.getLocalBounds();
    instructionText.setOrigin(textRect.width / 2, textRect.height / 2);
    instructionText.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 100);

    // Texte de contrôle pour le joueur 1
    player1ControlText.setFont(font);
    player1ControlText.setString("Player 1 controls: Z/S");
    player1ControlText.setCharacterSize(16);
    player1ControlText.setFillColor(sf::Color::White);
    sf::FloatRect player1Rect = player1ControlText.getLocalBounds();
    player1ControlText.setOrigin(player1Rect.width / 2, player1Rect.height / 2);
    player1ControlText.setPosition(window.getSize().x / 4, window.getSize().y / 2 - 50);

    // Texte de contrôle pour le joueur 2
    player2ControlText.setFont(font);
    player2ControlText.setString("Player 2 controls: Up/Down");
    player2ControlText.setCharacterSize(16);
    player2ControlText.setFillColor(sf::Color::White);
    sf::FloatRect player2Rect = player2ControlText.getLocalBounds();
    player2ControlText.setOrigin(player2Rect.width / 2, player2Rect.height / 2);
    player2ControlText.setPosition(3 * window.getSize().x / 4, window.getSize().y / 2 - 50);

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

void Game::receiveMessages() {
    char buffer[1024];
    SOCKET clientSocket = getClientSocket();
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::cout << "Message from server: " << buffer << "\n";
            Game::processServerCommand(buffer);
        }
        else if (bytesReceived == 0) {
            std::cout << "Server disconnected.\n";
            break;
        }
        else {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                std::cerr << "Recv failed with error: " << error << "\n";
                break;
            }
            // Else it is WSAEWOULDBLOCK, no data available yet, continue the loop
        }
    }
    closesocket(clientSocket); // Make sure to close the socket when finished
    WSACleanup();
}

void Game::processServerCommand(char command[1024]) {
    std::cout << command;
    if (strcmp(command, "Z") == 0) {
        isMovingUp = true;  // Exemple de commande, ajustez selon le gameplay
    }
    else if (strcmp(command, "stopZ") == 0) {
        isMovingUp = false;  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    else if (strcmp(command, "S") == 0) {
        isMovingDown = true;  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    else if (strcmp(command, "stopS") == 0) {
        isMovingDown = false;  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    else if (strcmp(command, "U") == 0) {
        isMovingLeft = true;  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    else if (strcmp(command, "stopU") == 0) {
        isMovingLeft = false;  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    else if (strcmp(command, "D") == 0) {
        isMovingRight = true;  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    else if (strcmp(command, "stopD") == 0) {
        isMovingRight = false;  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    else if (strcmp(command, "Space") == 0) {
        ball.startMovement();  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    // Ajoutez d'autres commandes selon vos besoins
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
    
        /*std::cout <<  (window.getSize().y/2) - (leftPaddle.getSize().y/2) << std::endl;*/
        if (event.type == sf::Event::Closed) {

            window.close();
            if (&Menu::isConnected) {
                &Menu::setNotConnected;
            }
        }

        if (event.type == sf::Event::KeyPressed) {

            handlePlayerInput(event.key.code, true);
        }

        else if (event.type == sf::Event::KeyReleased) {
            handlePlayerInput(event.key.code, false);

        }
    }
}

void Game::update(sf::Time deltaTime) {
    if (isMovingUp) {
        leftPaddle.moveUp(deltaTime);
    }
    if (isMovingDown) {
        leftPaddle.moveDown(deltaTime, window.getSize().y);
    }
    if (isMovingLeft) {
        rightPaddle.moveUp(deltaTime);
    }
    if (isMovingRight) {
        rightPaddle.moveDown(deltaTime, window.getSize().y);
    }

    ball.update(deltaTime, leftPaddle, rightPaddle, window.getSize().y);

    // Récupérer la position x de la balle et son rayon pour déterminer si elle a franchi les limites
    float ballX = ball.getPosition().x;
    float ballRadius = ball.getRadius();

    // Vérifie si la balle passe la limite gauche de la fenêtre
    if (ballX + ballRadius < 0) {  // Si le centre de la balle plus son rayon est inférieur à 0, elle est passée à gauche
        scoreManager.addScoreRight();  // Ajoute un point au joueur de droite
        ball.resetPosition(320, 240);  // Réinitialiser la position de la balle au centre de la fenêtre
    } 
    // Vérifie si la balle passe la limite droite de la fenêtre
    else if (ballX - ballRadius > window.getSize().x) {  // Si le centre de la balle moins son rayon dépasse la largeur de la fenêtre, elle est passée à droite
        scoreManager.addScoreLeft();  // Ajoute un point au joueur de gauche
        ball.resetPosition(320, 240);  // Réinitialiser la balle
    }

}

void Game::render() {
    window.clear();
    leftPaddle.draw(window);
    rightPaddle.draw(window);
    scoreManager.draw(window);

    if (!ball.isMoving) {
        window.draw(instructionText);
        window.draw(player1ControlText);
        window.draw(player2ControlText);
    }
    
    ball.draw(window);
    window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    //if (!isPressed) return;
    SOCKET TestSocket = getClientSocket();
    
    switch (key) {
    case sf::Keyboard::Space:
        /*ball.startMovement();*/
        sendToServer(TestSocket, "Space");
        break;
    case sf::Keyboard::Z: {
        /*isMovingUp = isPressed;*/
        if (isPressed) {
            sendToServer(TestSocket, "Z");
        }
        else {
            sendToServer(TestSocket, "stopZ");
        }
        break;
    }
    case sf::Keyboard::S: {
        /*isMovingDown = isPressed;*/
        if (isPressed) {
            sendToServer(TestSocket, "S");
        }
        else {
            sendToServer(TestSocket, "stopS");
        }
        break;
    }
    case sf::Keyboard::Up: {
        /*isMovingLeft = isPressed;*/
        if (isPressed) {
            sendToServer(TestSocket, "U");
        }
        else {
            sendToServer(TestSocket, "stopU");
        }
        break;
    }
    case sf::Keyboard::Down: {
        /*isMovingRight = isPressed;*/
        if (isPressed) {
            sendToServer(TestSocket, "D");
        }
        else {
            sendToServer(TestSocket, "stopD");
        }
        break;
    }
    }
}
