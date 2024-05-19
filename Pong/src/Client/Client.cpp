#include "../../includes/Client/Client.h"
#include <iostream>
#include <thread>
#include <ws2tcpip.h>
#include <conio.h>  // For _kbhit() and _getch()

#pragma comment(lib, "Ws2_32.lib")

const char* SERVER_IP = "192.168.1.11";  // Server IP address depending on the network
const int SERVER_PORT = 2003;  // Server port

SOCKET clientSocket = INVALID_SOCKET;


//void receiveMessages() {
//    char buffer[1024];
//    while (true) {
//        memset(buffer, 0, sizeof(buffer));
//        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//        if (bytesReceived > 0) {
//            std::cout << "Message from server: " << buffer << "\n";
//            Game::processServerCommand(buffer);
//        }
//        else if (bytesReceived == 0) {
//            std::cout << "Server disconnected.\n";
//            break;
//        }
//        else {
//            int error = WSAGetLastError();
//            if (error != WSAEWOULDBLOCK) {
//                std::cerr << "Recv failed with error: " << error << "\n";
//                break;
//            }
//            // Else it is WSAEWOULDBLOCK, no data available yet, continue the loop
//        }
//    }
//    closesocket(clientSocket); // Make sure to close the socket when finished
//    WSACleanup();
//}

void connectToServer() {
    WSADATA wsaData;
    // Initialisation de Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return;
    }

    // Vérification de la création du socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return;
    }

    // Définir le socket en mode non-bloquant
    u_long mode = 1;  // 1 pour activer le mode non bloquant
    if (ioctlsocket(clientSocket, FIONBIO, &mode) != 0) {
        std::cerr << "Failed to set non-blocking mode: " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    // Conversion de l'adresse IP en format binaire
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address\n";
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSAEWOULDBLOCK) {
            std::cerr << "Connection failed: " << error << "\n";
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
        // Connection is in progress
        fd_set writeSet;
        FD_ZERO(&writeSet);
        FD_SET(clientSocket, &writeSet);

        timeval timeout;
        timeout.tv_sec = 5;  // Set timeout to 5 seconds
        timeout.tv_usec = 0;

        int selectResult = select(0, NULL, &writeSet, NULL, &timeout);
        if (selectResult > 0) {
            int optval;
            int optlen = sizeof(optval);
            getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen);
            if (optval == 0) {
                std::cout << "Connection successfully established\n";
            }
            else {
                std::cerr << "Connection failed: " << optval << "\n";
                closesocket(clientSocket);
                WSACleanup();
                return;
            }
        }
        else if (selectResult == 0) {
            std::cerr << "Connection timeout\n";
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
        else {
            std::cerr << "Select error: " << WSAGetLastError() << "\n";
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
    }

    std::cout << "Connected to server on port " << SERVER_PORT << "\n";
    std::thread receiverThread(&Game::receiveMessages, Game::GetInstance());
    receiverThread.detach();  // Ensure the thread continues to run
}

void sendToServer(SOCKET sock, const std::string& message) {
    send(sock, message.c_str(), message.size(), 0);
}

SOCKET getClientSocket() {
    return clientSocket;  // Exposes the global SOCKET for external use
}
