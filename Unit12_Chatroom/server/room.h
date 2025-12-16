#ifndef ROOM_H
#define ROOM_H
#include "user.h"
#include <string>
#include <vector>
class Room {
public:
    std::string name;
    std::vector<User*> users;

    Room(const std::string& name) : name(name) {}

    void addUser(User* user) {
        users.push_back(user);
    }

    void removeUser(User* user) {
        users.erase(std::remove(users.begin(), users.end(), user), users.end());
    }
};
#endif
