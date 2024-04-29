#include "../../includes/Client/Client.h"
#include <iostream>
//#include <cstdlib>
//#include <cstring>
#include <thread>
#include <ws2tcpip.h>
#include <conio.h>  // For _kbhit() and _getch()

#pragma comment(lib, "Ws2_32.lib")

const char* SERVER_IP = "192.168.0.17";  // Server IP address
const int SERVER_PORT = 2003;          // Server port

void receiveMessages(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cout << "Server disconnected or error occurred.\n";
            break;
        }
        std::cout << "Message from server: " << buffer << "\n";
    }
}

void connectToServer() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << "\n";
        WSACleanup();
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address\n";
        closesocket(clientSocket);
        WSACleanup();
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect: " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
    }

    std::cout << "Connected to server on port 2003\n";
    std::cout << "Press 'Z' to send 'Hello!' to the server or ESC to quit.\n";

    std::thread receiverThread(receiveMessages, clientSocket);
    receiverThread.detach();

    /*closesocket(clientSocket);
    WSACleanup();*/
}
