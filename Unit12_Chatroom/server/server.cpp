#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <random>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif

std::vector<int> clients;
std::mutex clientsMutex;

class ChatServer {
private:
	private:
    std::map<int, User*> users;                 // socket → User
    std::map<std::string, Room*> rooms;         // room name → Room
    std::mutex mtx;

    std::string generateAnonName() {
        static std::mt19937 rng{ std::random_device{}() };
        return "anon" + std::to_string(rng() % 10000);
    }

    bool usernameExists(const std::string& name) {
        for (auto& [_, user] : users)
            if (user->username == name)
                return true;
        return false;
    }

public:
    ChatServer() {
        rooms["Lobby"] = new Room("Lobby");
    }

    void broadcastToRoom(const std::string& room, const std::string& msg, int exclude = -1) {
        for (auto* user : rooms[room]->users) {
            if (user->socket != exclude) {
                send(user->socket, msg.c_str(), msg.size(), 0);
            }
        }
    }

    void handleClient(int clientSocket) {
        char buffer[4096];

        /* ---- Receive username ---- */
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::string requestedName(buffer);

        std::lock_guard<std::mutex> lock(mtx);

        if (requestedName.empty() || usernameExists(requestedName)) {
            requestedName = generateAnonName();
        }

        User* user = new User(clientSocket, requestedName);
        users[clientSocket] = user;
        rooms["Lobby"]->addUser(user);

        std::string welcome = "[SERVER] Username assigned: " + user->username + "\n";
        send(clientSocket, welcome.c_str(), welcome.size(), 0);

        broadcastToRoom("Lobby", "[SERVER] " + user->username + " joined Lobby\n", clientSocket);

        while (true)
        {
            memset(buffer, 0, sizeof(buffer));
            int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) break;

            std::string msg(buffer);
            processCommand(user, msg);
        }

        broadcastToRoom(user->room, "[SERVER] " + user->username + " left the room\n", clientSocket);

        rooms[user->room]->removeUser(user);
        users.erase(clientSocket);
        closesocket(clientSocket);
        delete user;
    }

    void processCommand(User* user, const std::string& msg) {
        std::istringstream iss(msg);
        std::string command;
        iss >> command;

        if (command == ".CREATE_ROOM") {
            std::string roomName;
            iss >> roomName;
            if (!roomName.empty() && rooms.count(roomName) == 0) {
                rooms[roomName] = new Room(roomName);
                send(user->socket, "[SERVER] Room created\n", 23, 0);
            }
        }
        else if (command == ".JOIN_ROOM") {
            std::string roomName;
            iss >> roomName;
            if (rooms.count(roomName)) {
                rooms[user->room]->removeUser(user);
                broadcastToRoom(user->room, "[SERVER] " + user->username + " left\n", user->socket);

                user->room = roomName;
                rooms[roomName]->addUser(user);
                broadcastToRoom(roomName, "[SERVER] " + user->username + " joined\n");
            }
        }
        else if (command == ".LIST_ROOMS") {
            std::string list = "[SERVER] Rooms: ";
            for (auto& [name, _] : rooms) list += name + " ";
            list += "\n";
            send(user->socket, list.c_str(), list.size(), 0);
        }
        else if (command == ".EXIT") {
            closesocket(user->socket);
        }
        else {
            broadcastToRoom(user->room, "[" + user->username + "] " + msg, user->socket);
        }
    }
};

int main(int argc, char* argv[]) {
	std::string ipAddress = "127.0.0.1";
    uint16_t port = 54000;
	if (argc >= 2) ip = argv[1];
    if (argc >= 3) port = std::stoi(argv[2]);

#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif

    int listening = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));
    listen(listening, SOMAXCONN);

	while(true) {
    sockaddr_in client{};
    socklen_t clientSize = sizeof(client);
	
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	std::thread(&ChatServer::handleClient, &chatServer, clientSocket).detach();
	}
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
} 
