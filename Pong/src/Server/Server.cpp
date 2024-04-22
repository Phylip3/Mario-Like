#include "../../includes/Server/Server.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

std::vector<SOCKET> clients;
std::mutex clients_mutex;

void handle_client(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Connection closed or error\n";
            break;
        }

        std::lock_guard<std::mutex> guard(clients_mutex);
        for (SOCKET outSock : clients) {
            if (outSock != clientSocket) {
                send(outSock, buffer, bytesReceived, 0);
            }
        }
    }

    {
        std::lock_guard<std::mutex> guard(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
        closesocket(clientSocket);
    }
}

void startServer() {
    WSADATA WSAData;
    SOCKET serverSocket;
    SOCKADDR_IN serverAddr;

    WSAStartup(MAKEWORD(2, 0), &WSAData);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2003);

    bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 0);
    std::cout << "Listening for incoming connections..." << std::endl;

    while (true) {
        SOCKADDR_IN clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize);
        if (clientSocket != INVALID_SOCKET) {
            std::cout << "Client connected!" << std::endl;
            std::thread clientThread(handle_client, clientSocket);
            clientThread.detach();
        }
        else {
            std::cerr << "Failed to accept client\n";
        }
    }

    closesocket(serverSocket);
    WSACleanup();
}
