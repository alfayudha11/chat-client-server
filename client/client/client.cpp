#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(11111); // Use the same port number as the server
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use the server's IP address

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the server." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char buffer[BUFFER_SIZE];
    while (true) {
        std::cout << "Enter a message (or 'exit' to quit): ";
        std::cin.getline(buffer, BUFFER_SIZE);

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        send(clientSocket, buffer, strlen(buffer), 0);

        int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead <= 0) {
            std::cerr << "Server disconnected." << std::endl;
            break;
        }
        buffer[bytesRead] = '\0';
        std::cout << "Received from server: " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
