#include <iostream>
#include <cstring>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsData;
    if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET6, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in6 serverAddr;
    serverAddr.sin6_family = AF_INET6;
    serverAddr.sin6_port = htons(12345); // Port number

    // Replace "your_server_ipv6_address" with the server's IPv6 address
    inet_pton(AF_INET6, "your_server_ipv6_address", &(serverAddr.sin6_addr));

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char message[1024];

    while (true) {
        std::cout << "Enter a message (or type 'quit' to exit): ";
        std::cin.getline(message, sizeof(message));

        if (strcmp(message, "quit") == 0) {
            break;
        }

        send(clientSocket, message, strlen(message), 0);

        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error in recv()." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Server says: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
