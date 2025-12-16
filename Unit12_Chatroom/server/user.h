class User {
public: 
	std::string userName, roomName;
    int socket;

	User(int sock, const std::string& name) : socket(sock), userName(name), roomName("Lobby") {}
};
