#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include <mutex>
#include <algorithm>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define SOCKET_TYPE SOCKET
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#define SOCKET_TYPE int
#endif

std::vector<SOCKET_TYPE> clients;
std::mutex clientsMutex;

/* Broadcast message to all clients except sender */
void broadcast(const std::string& msg, SOCKET_TYPE sender)
{
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (SOCKET_TYPE client : clients)
    {
        if (client != sender)
            send(client, msg.c_str(), msg.size(), 0);
    }
}

void handleClient(SOCKET_TYPE client)
{
    char buffer[4096];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client, buffer, sizeof(buffer), 0);

        if (bytes <= 0)
            break;

        broadcast(std::string(buffer, bytes), client);
    }

    /* Remove client */
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.erase(std::remove(clients.begin(), clients.end(), client),
                      clients.end());
    }

    closesocket(client);
}

int main(int argc, char* argv[])
{
#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif

    std::string ip   = (argc >= 2) ? argv[1] : "127.0.0.1";
    int port         = (argc >= 3) ? std::stoi(argv[2]) : 54000;

    SOCKET_TYPE listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening < 0)
    {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    int opt = 1;
    setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &hint.sin_addr);

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    if (listen(listening, SOMAXCONN) < 0) {
        std::cerr << "Listen failed\n";
        return 1;
    }

    std::cout << "Server listening on " << ip << ":" << port << std::endl;

    while (true)
    {
        sockaddr_in client{};
        socklen_t size = sizeof(client);

        SOCKET_TYPE clientSocket = accept(listening, (sockaddr*)&client, &size);
        if (clientSocket < 0)
            continue;
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            clients.push_back(clientSocket);
        }
        std::thread(handleClient, clientSocket).detach();
    }

    closesocket(listening);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
