#include "Client.h"
#include "QuickSort.h"
#include <winsock2.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

void generateRandomArray(std::vector<int>& arr, size_t size, int range) {
    for (size_t i = 0; i < size; ++i) {
        arr.push_back(rand() % range);
    }
}

void printArray(const std::vector<int>& arr) {
    std::cout << std::endl;
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void connectToServer(const std::string& serverAddress, int serverPort, size_t arraySize, int numThreads, int range) {
    WSADATA wsaData;
    int wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaStartup != 0) {
        std::cerr << "WSAStartup failed with error: " << wsaStartup << std::endl;
        return;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
    serverAddr.sin_port = htons(serverPort);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed with error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Connected to server." << std::endl;

   

    // Generate a random array
    std::vector<int> arr;
    srand(static_cast<unsigned int>(time(0)));
    generateRandomArray(arr, arraySize, range);

    std::cout << "Generated array.\n";
    printArray(arr);
    auto start = std::chrono::high_resolution_clock::now();

    int max_depth = numThreads;  // You can adjust the depth based on the number of threads
    quicksort_parallel(0, arraySize -1, arr, max_depth);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sorting_duration = end - start;
    std::cout << "The array was sorted in " << sorting_duration.count() << " seconds." << std::endl;
    printArray(arr);


    // Send array size
    if (send(clientSocket, (char*)&arraySize, sizeof(arraySize), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send array size: " << WSAGetLastError() << std::endl;
    }

    // Send number of threads
    if (send(clientSocket, (char*)&numThreads, sizeof(numThreads), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send number of threads: " << WSAGetLastError() << std::endl;
    }

    // Send array data
    if (send(clientSocket, (char*)arr.data(), arraySize * sizeof(int), 0) == SOCKET_ERROR) {
        std::cerr << "Failed to send array: " << WSAGetLastError() << std::endl;
    }


       
    closesocket(clientSocket);
    WSACleanup();
}
