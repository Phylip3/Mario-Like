#ifndef CLIENT_H
#define CLIENT_H
#define NOMINMAX

#include <winsock2.h>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../../includes/Game.h"
#include "../../includes/CoreGame/Menu.h"

// Constants for server connection
extern const char* SERVER_IP;  // Server IP address
extern const int SERVER_PORT;  // Server port

// Function to handle incoming messages
void receiveMessages();
void connectToServer();
void sendToServer(SOCKET sock, const std::string& message);
SOCKET getClientSocket();

#endif // CLIENT_H
