#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <map>
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
#endif

std::vector<int> clients;
std::mutex clientsMutex;

//class for users
class User {
public: 
	std::string userName, roomName;
    int socket;

	User(int sock, const std::string& name) : socket(sock), userName(name), roomName("Lobby") {}
};

//class for rooms
class Room {
public:
    std::string roomname;
    std::vector<User*> users;
    Room(const std::string& name) : roomname(name) {};

    std::string getRoomName() {
        return roomname;
	}
    void addUser(User* user) {
        users.push_back(user);
    }
    void removeUser(User* user) {
        users.erase(std::remove(users.begin(), users.end(), user), users.end());
    }
};
