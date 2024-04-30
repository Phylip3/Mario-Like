#include "../includes/Game.h"
#include "../includes/Client/Client.h"
#include "../includes/CoreGame/Menu.h"
#include <iostream>

Game::Game() : window(sf::VideoMode(640, 480), "SFML Pong"), leftPaddle( 10 , (window.getSize().y/2) - 50), rightPaddle(window.getSize().x - 20, window.getSize().y/2 - 50), ball(window.getSize().x/2, window.getSize().y/2), scoreManager(window)
{
}

void Game::run() {
    sf::Clock clock;
    serverThread = std::thread(&Game::receiveServerMessages, this);

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::receiveServerMessages() {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived > 0) {
            std::cout << "Received message: " << buffer << std::endl;
            std::string message(buffer);
            processServerCommand(message);
        }
        else if (bytesReceived == 0) {
            std::cout << "Server disconnected.\n";
            break;  // Sortie de la boucle pour arrêter le thread proprement
        }
        else {
            int error = WSAGetLastError();
            std::cerr << "recv failed with error: " << error << std::endl;

            // Traiter des erreurs spécifiques si nécessaire
            if (error == WSAECONNRESET) {
                std::cerr << "Connection reset by peer.\n";
            }
            else if (error == WSAENOTSOCK) {
                std::cerr << "Socket operation on non-socket.\n";
            }
            else if (error == WSAENOTCONN) {
                std::cerr << "Socket is not connected.\n";
            }

            // Selon l'erreur, vous pourriez vouloir fermer la connexion ou essayer de récupérer
            break;  // Sortie de la boucle pour arrêter le thread proprement en cas d'erreur
        }
    }
}



void Game::processServerCommand(const std::string& command) {
    if (command == "Z") {
        isMovingUp = true;  // Exemple de commande, ajustez selon le gameplay
    }
    else if (command == "StopZ") {
        isMovingUp = false;  // Assurez-vous d'arrêter le mouvement lorsque la touche est relâchée
    }
    // Ajoutez d'autres commandes selon vos besoins
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
    
        /*std::cout <<  (window.getSize().y/2) - (leftPaddle.getSize().y/2) << std::endl;*/
        if (event.type == sf::Event::Closed) {

            window.close();
            std::terminate();
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
    ball.draw(window);
    window.display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    //if (!isPressed) return;
    
    switch (key) {
    case sf::Keyboard::Space:
        ball.startMovement();
        break;
    case sf::Keyboard::Z: {
        /*isMovingUp = isPressed;*/
        sendToServer(clientSocket, "Z");
        break;
    }
    case sf::Keyboard::S: {
        /*isMovingDown = isPressed;*/
        sendToServer(clientSocket, "S");
        break;
    }
    case sf::Keyboard::Up: {
        /*isMovingLeft = isPressed;*/
        sendToServer(clientSocket, "U");
        break;
    }
    case sf::Keyboard::Down: {
        /*isMovingRight = isPressed;*/
        sendToServer(clientSocket, "D");
        break;
    }
    }
}
