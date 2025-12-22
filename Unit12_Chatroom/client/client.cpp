#include <iostream>
#include <thread>
#include <string>
#include <atomic>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
using socket_t = SOCKET;
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
using socket_t = int;
#define closesocket close
#endif

std::atomic<bool> running(true);
const int b_size = 4096;
// receive message;
void receivemessage(socket_t sock) {
    char buffer[b_size-1];
    while (running) {
#ifdef _WIN32
        int bytes = recv(sock, buffer, sizeof(buffer)-1, 0);
#else
        int bytes = read(sock, buffer, sizeof(buffer)-1);
#endif
        
        if (bytes <= 0) {
            running = false;
            break;
        }
        buffer[bytes] = '\0';
        std::cout << std::string(buffer, bytes) << std::endl;
        std::cout.flush();
    }
}
//print supporting commands
void printCommands() {
    std::printf("Supported Commands:\n");
    std::printf("\t.HELP \t\t\t\t\t Show supported commands\n");
    std::printf("\t.CREATE_ROOM <name>\t\t\t to create a new room\n");
    std::printf("\t.JOIN_ROOM <name>\t\t\t to join another room\n");
    std::printf("\t.LIST_ROOMS\t\t\t\t to print list of rooms\n");
    std::printf("\t.EXIT\t\t\t\t\t to exit\n");
    std::printf("Any other text is sent as a chat message.\n");
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data)!= 0) {
        std::cerr << "WSAStartup failed..." << std::endl;
        return 1;
    }
#endif
    //declaring ipaddress, port#, username
    std::string ip = "127.0.0.1", username = "ANONYMOUS";
    uint16_t port = 54000;
    if (argc >= 2) ip = argv[1];
    if (argc >= 3) port = std::stoi(argv[2]);
    if (argc >= 4) username = argv[3];
    //creating a TCP socket
    socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverHint{};
    serverHint.sin_family = AF_INET;
    serverHint.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverHint.sin_addr);
    //establish connection using socket
    int conn = connect(sock, (struct sockaddr*)&serverHint, sizeof(serverHint));
    //check for connection
    if (conn == -1) {
#ifdef _WIN32
        WSACleanup();
#endif
        std::cerr << "connection failed.\n\n" << std::endl;
        return 1;
    } else {
        std::cerr << "Connection established.....\n\n" << std::endl;
        std::cerr << "Connected to " << ip << ":" << port << std::endl;
    }
    std::string handshake = "Username: " + username;
    //handshake
    send(sock, handshake.c_str(), handshake.size(), 0);
    std::string input;
    
    std::thread receive(receivemessage, sock);
    //welcome to chat message
    std::printf("Welcome: %s\n\n", username.c_str());
    
    printCommands();
    
    while(running) {
        std::cout <<  username.c_str() << ": ";
        getline(std::cin, input);
        if (input == ".EXIT") {
            send(sock, input.c_str(), input.size(), 0);
            running = false;
            break;
        } else if (input == ".HELP") {
            printCommands();
        } else {
            input += '\n';
            send(sock, input.c_str(), input.size(), 0);
        }
        
    }
    
    if (receive.joinable()) receive.join();
    closesocket(sock);
    
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
