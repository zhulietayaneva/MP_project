#include "Client.h"
#include "QuickSort.h"
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")



void connectToServer(const std::string& serverAddress, int serverPort, size_t arraySize, int numThreads, int range) {
    WSADATA wsaData;
    int wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaStartup != 0) {
        std::cerr << "Client: WSAStartup failed with error: " << wsaStartup << std::endl;
        return;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Client: Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
    serverAddr.sin_port = htons(serverPort);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Client: Connection to server failed with error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Client: Connected to server." << std::endl;

   

    


    // Send array size
    if (send(clientSocket, (char*)&arraySize, sizeof(arraySize), 0) == SOCKET_ERROR) {
        std::cerr << "Client: Failed to send array size: " << WSAGetLastError() << std::endl;
    }

    // Send number of threads
    if (send(clientSocket, (char*)&numThreads, sizeof(numThreads), 0) == SOCKET_ERROR) {
        std::cerr << "Client: Failed to send number of threads: " << WSAGetLastError() << std::endl;
    }
      
    
    // Send the range of numbers
    if (send(clientSocket, (char*)&range, sizeof(range), 0) == SOCKET_ERROR) {
        std::cerr << "Client: Failed to send range of numbers: " << WSAGetLastError() << std::endl;
    }

  


    std::vector<int> sortedArray(arraySize);
    // Step 3: Receive the actual sorted array data
    if (recv(clientSocket, (char*)sortedArray.data(), arraySize * sizeof(int), 0) <= 0) {
        std::cerr << "Client: Failed to receive sorted array data: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        return;
    }
    if (arraySize<=100)
    {
     std::cout << "Client: Printing array..." << std::endl;
     printArray(sortedArray);
    }



       
    closesocket(clientSocket);
    WSACleanup();
}
