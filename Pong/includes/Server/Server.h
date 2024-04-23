#ifndef SERVER_H
#define SERVER_H
#define NOMINMAX

#include <vector>
#include <mutex>
#include <winsock2.h>

struct Client {
    SOCKET socket;
    int id;
};

extern std::vector<Client> clients;
extern std::mutex clients_mutex;
extern volatile bool serverShouldStop;
extern SOCKET serverSocket;

void handle_client(Client client);
void startServer();
void stopServer();

#endif // SERVER_H
