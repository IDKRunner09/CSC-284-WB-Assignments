#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <thread>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#define SOCKET int
#endif

std::vector<SOCKET> clients;
std::mutex clientMutex;

void broadcast(const std::string &msg, SOCKET sender) {
    std::lock_guard<std::mutex>lock(clientMutex);
    for (SOCKET client : clients) {
        if(client != sender) {
            send(client, msg.c_str(), msg.size(), 0);
        }
    }
}

void handleClient(SOCKET clientSocket) {
    char buffer[4096];
    
    while(true) {
        memset(buffer, 0, sizeof(buffer));
        auto byteReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (byteReceived <= 0) break;
        
        std::string msg(buffer, byteReceived);
        broadcast(msg, clientSocket);
    }
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    }
    closesocket(clientSocket);
}

int main(int argc, char*argv[]) {
#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif
    std::string ip = (argc >= 2) ? argv[1] : "127.0.0.1";
    uint16_t port = (argc >= 3) ? std::stoi(argv[2]) : 54000;
    
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening < 0)
    {
        std::cerr << "Cannot create socket";
        return 1;
    }
    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = INADDR_ANY;

    bind(listening, (sockaddr *)&hint, sizeof(hint));
    listen(listening, SOMAXCONN);

    std::cout << "Server running on " << ip << ":" << port << std::endl;
    
    while(true) {
        sockaddr_in client{};
        socklen_t clientSize = sizeof(client);
        
        SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if (clientSocket < 0) continue;
        {
            std::lock_guard<std::mutex> lock(clientMutex);
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
