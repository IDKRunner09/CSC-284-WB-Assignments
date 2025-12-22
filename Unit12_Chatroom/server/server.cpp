#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstring>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using socket_t = SOCKET;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#define closesocket close
using socket_t = int;
#endif


struct User {
    socket_t u_sock;
    std::string username, roomname;
};

class Room{
private:
    std::string name;
    std::vector<socket_t> users;
public:
    Room() = default;
    Room(std::string n) : name(n) {}
    
    void addUser(socket_t sock) {
        if(std::find(users.begin(), users.end(), sock) == users.end()) {
            users.push_back(sock);
        }
    }
    
    void removeUser(socket_t sock) {
        users.erase(std::remove(users.begin(), users.end(), sock), users.end());
    }
    
    std::vector<socket_t>& getusers() {
        return users;
    }
};

class ChatServer {
private:
    std::map<socket_t, User> users;
    std::map<std::string, Room> rooms;
    std::mutex s_mutex;
    
    std::string trim(const std::string& s) {
            size_t a = s.find_first_not_of(" \n\r\t");
            size_t b = s.find_last_not_of(" \n\r\t");
            if (a == std::string::npos) return "";
            return s.substr(a, b - a + 1);
    }
    
    std::string assignUsername(const std::string& requested) {
        std::string base = requested.empty() ? "ANONYMOUS" : requested;
        std::string name = base;
        int i = 1;

        while (true) {
            bool taken = false;
            for (auto& [_, u] : users) {
                if (u.username == name) {
                    taken = true;
                    name = base + std::to_string(i++);
                    break;
                }
            }
            if (!taken) break;
        }
        return name;
    }
    
    void sendTo(socket_t s, const std::string& msg) {
            send(s, msg.c_str(), msg.size(), 0);
        }

    void broadcast(const std::string& room, const std::string& msg, socket_t except) {
        for (socket_t s : rooms[room].getusers()) {
            if (s != except) sendTo(s, msg);
        }
    }
    
    void listRooms(socket_t s) {
            sendTo(s, "[Server] Available rooms:\n");
            for (auto& [name, _] : rooms) sendTo(s, " - " + name + "\n");
        }
    
    
    void handleClient(socket_t clientsocket) {
        char buffer[4096];
        
#ifdef _WIN32
            int bytes = recv(clientsocket, buffer, sizeof(buffer), 0);
#else
            int bytes = read(clientsocket, buffer, sizeof(buffer));
#endif
        if (bytes <= 0) {
            closesocket(clientsocket);
            return;
        }
        std::lock_guard<std::mutex> lock(s_mutex);
        std::string name (buffer,bytes);
        std::string username = assignUsername(trim(name));
        
        std::cerr << username << " joined!" << std::endl;
        
        User user;
        user.username = username;
        user.u_sock = clientsocket;
        user.roomname = "Lobby";
        
        users.emplace(clientsocket, user);
        rooms["Lobby"].addUser(clientsocket);
        
        sendTo(clientsocket, "[Server] Welcome " + username + "! You are in Lobby.\n");
        broadcast("Lobby", "[Server] " + username + " joined Lobby.\n", clientsocket);

        while(true) {
            std::memset(buffer, 0, sizeof(buffer));
#ifdef _WIN32
            bytes = recv(clientsocket, buffer, sizeof(buffer), 0);
#else
            bytes = read(clientsocket, buffer, sizeof(buffer));
#endif
            if (bytes <= 0) {
                std::cerr << username << "disconnected..." << std::endl;
                break;
            }
            
            std::string msg = trim(std::string(buffer,bytes));
            if (msg.empty()) continue;
            
            if(msg[0] == '.') {
                std::stringstream iss(msg);
                std::string command;
                iss >> command;
                
                if (command == ".CREATE_ROOM") {
                    std::string roomname;
                    iss >> roomname;
                    
                    if (!roomname.empty() && !rooms.count(roomname)) {
                        rooms.emplace(roomname, Room(roomname));
                        std::cerr << roomname << " created." << std::endl;
                        sendTo(clientsocket, "Room created.\n");
                        
                    }
                } else if (command == ".JOIN_ROOM") {
                    std::string room_n;
                    iss >> room_n;
                    
                    if (!rooms.count(room_n)) {
                        sendTo(clientsocket, "Server: Room does not exist.\n");
                        continue;
                    }
                    //leaving old room
                    std::string oldroom = users[clientsocket].roomname;
                    rooms[oldroom].removeUser(clientsocket);
                    
                    sendTo(clientsocket, room_n + " left.\n");
                    broadcast(oldroom, "Server: " + username + " has left room.\n", clientsocket);
                    
                    //join requested room
                    users[clientsocket].roomname = room_n;
                    rooms[room_n].addUser(clientsocket);
                    sendTo(clientsocket, "Server: " + room_n + " joined.\n");
                    broadcast(room_n, "Server: " + username + " has joined room.\n", clientsocket);
                } else if (command == ".LIST_ROOMS") {
                    listRooms(clientsocket);
                } else if (command == ".EXIT") {
                    // std::cerr << username << " disconnected." << std::endl;
                    break;
                } else {
                    sendTo(clientsocket, "Server: Unknown command.\n");
                }
            } else {
                broadcast(users[clientsocket].roomname, username + ": " + msg + "\n", clientsocket);
            }
        }
        std::string room = users[clientsocket].roomname;
        rooms[room].removeUser(clientsocket);
        users.erase(clientsocket);
        broadcast(room, "[Server] " + username + " disconnected.\n", clientsocket);
        closesocket(clientsocket);
    }
public:
    ChatServer() = default;
    
    void start(std::string ip, uint16_t port) {
        socket_t listening = socket(AF_INET, SOCK_STREAM, 0);
        if (listening < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }
        
        int opt = 1;
        setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
        
        sockaddr_in serverHint{};
        serverHint.sin_family = AF_INET;
        serverHint.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &serverHint.sin_addr);
        
        if (bind(listening, reinterpret_cast<sockaddr*>(&serverHint), sizeof(serverHint)) == -1) {
            std::cerr << "Binding Failed." << std::endl;
            return;
        }
        listen(listening, SOMAXCONN);
        
        std::cout << "Server listening on " << ip << ":" << port << std::endl;
        
        {
            std::lock_guard<std::mutex>lock(s_mutex);
            if (rooms.find("Lobby") == rooms.end()) {
                rooms.emplace("Lobby", Room("Lobby"));
            }
        }
        
        char buffer[4096];
        while (true)
        {
            //accepting a client
            sockaddr_in client{};
            socklen_t clientSize = sizeof(client);

            socket_t clientSocket = accept(listening, reinterpret_cast<struct sockaddr*>(&client), &clientSize);
            
            std::thread t(&ChatServer::handleClient, this, clientSocket);
            t.detach();
        }
        closesocket(listening);
    }
};


int main(int argc, char* argv[]) {
#ifdef _WIN32
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif
    //assigning ipaddress and port#
    std::string ip = "127.0.0.1";
    uint16_t port = 54000;
    
    if (argc >= 2) ip = argv[1];
    if (argc >= 3) port = std::stoi(argv[2]);
    
    ChatServer server;
    server.start(ip, port);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
