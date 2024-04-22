#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <mutex>
#include <winsock2.h>

// Global variables for maintaining the list of connected clients
extern std::vector<SOCKET> clients;
extern std::mutex clients_mutex;

// Function declarations
void handle_client(SOCKET clientSocket);
void startServer();  // Declare the function that starts the server

#endif // SERVER_H

