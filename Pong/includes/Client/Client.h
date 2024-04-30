#ifndef CLIENT_H
#define CLIENT_H
#define NOMINMAX

#include <winsock2.h>
#include <iostream>
#include <SFML/Graphics.hpp>

// Constants for server connection
extern const char* SERVER_IP;  // Server IP address
extern const int SERVER_PORT;  // Server port

extern SOCKET clientSocket;

// Function to handle incoming messages
void receiveMessages(SOCKET clientSocket);
void connectToServer();
void sendToServer(SOCKET sock, const std::string& message);

#endif // CLIENT_H
