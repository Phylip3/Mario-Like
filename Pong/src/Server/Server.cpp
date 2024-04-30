#include "../../includes/Server/Server.h"
#include "../../includes/Client/Client.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

std::vector<Client> clients;
std::mutex clients_mutex;
volatile bool serverShouldStop = false;
SOCKET serverSocket;

void handle_client(Client client) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client.socket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Connection closed or error\n";
            break;
        }

        // Afficher le message reçu dans la console du serveur
        std::cout << "Message received from client " << client.id << ": " << buffer << std::endl;

        std::lock_guard<std::mutex> guard(clients_mutex);
        for (const Client& other : clients) {
            if (other.socket != client.socket) {
                send(other.socket, buffer, bytesReceived, 0);
                std::cout << "Message send !";
            }
        }
    }

    {
        std::lock_guard<std::mutex> guard(clients_mutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(),
            [&client](const Client& c) { return c.socket == client.socket; }), clients.end());
        closesocket(client.socket);
    }
}

void startServer() {
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 0), &WSAData);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN serverAddr;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2003);

    bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 0);
    std::cout << "Listening for incoming connections..." << std::endl;

    while (!serverShouldStop && clients.size() < 2) {
        SOCKADDR_IN clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
        if (clientSocket != INVALID_SOCKET && clients.size() < 2) {
            std::cout << "Client connected!" << std::endl;
            Client newClient = { clientSocket, static_cast<int>(clients.size() + 1) };
            std::lock_guard<std::mutex> guard(clients_mutex);
            clients.push_back(newClient);
            std::thread clientThread(handle_client, newClient);
            clientThread.detach();
        }
        else {
            std::cerr << "Failed to accept client or server full\n";
            closesocket(clientSocket);  // Close the socket if the server is full
        }
    }

    closesocket(serverSocket);
    WSACleanup();
}

void stopServer() {
    closesocket(serverSocket);
    serverShouldStop = true;
    WSACleanup();
}