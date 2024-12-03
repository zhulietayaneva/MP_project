#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include "QuickSort.h"
#include "Server.h"
#include "Client.h"

#include <iostream>
#include <vector>
#include <string>




// Function to handle a single client connection
void handleClient(int clientSocket) {
    std::cout << "Server: Handling client..." << std::endl;

    // Send greeting to the client
  

    // Receive initial message from client
  

    // Process array size
    size_t size;
    if (recv(clientSocket, (char*)&size, sizeof(size), 0) <= 0) {
        std::cerr << "Server: Failed to receive array size: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        return;
    }
   

    // Process number of threads
    int numThreads;
    if (recv(clientSocket, (char*)&numThreads, sizeof(numThreads), 0) <= 0) {
        std::cerr << "Server: Failed to receive thread count: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        return;
    }
    std::cout << "Server: Number of threads received: " << numThreads << std::endl;

    // Process range
    int range;
    if (recv(clientSocket, (char*)&range, size * sizeof(range), 0) <= 0) {
        std::cerr << "Server: Failed to receive range: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        return;
    }
    std::cout << "Server: Range recieved: "<<size<<"\n";


    // Process array data
    std::vector<int> arr;
   

    // Generate a random array
    
    srand(static_cast<unsigned int>(time(0)));
    generateRandomArray(arr, size, range);

    std::cout << "Server: Generated array.\n";

    if (size <= 100)    {
        std::cout << "Server: Printing generated array..." << std::endl;
        printArray(arr);
    }

   
    auto start = std::chrono::high_resolution_clock::now();

    int max_depth = numThreads;  // You can adjust the depth based on the number of threads
    quicksort_parallel(0, size - 1, arr, max_depth);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sorting_duration = end - start;
    std::cout << "Server: The array was sorted in " << sorting_duration.count() << " seconds." << std::endl;
    //printArray(arr);


    // Send the sorted array back to the client
    if (send(clientSocket, (char*)arr.data(), size * sizeof(int), 0) == SOCKET_ERROR) {
        std::cerr << "Server: Failed to send sorted array: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        return;
    }
    std::cout << "Server: Sorted array sent back to client." << std::endl;

    // Close the client socket
    closesocket(clientSocket);
    std::cout << "Server: Client connection closed." << std::endl;
}

// Function to start the server
void startServer(const std::string& address, int port) {
    // Initialize Winsock
    WSADATA wsaData;
    int wsaStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaStartup != 0) {
        std::cerr << "Server: WSAStartup failed with error: " << wsaStartup << std::endl;
        return;
    }

    // Create a socket to listen for incoming connections
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Server: Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // Set up the sockaddr_in structure for binding
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(address.c_str());
    serverAddr.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Server: Bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections (maximum of 5 queued connections)
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Server: Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server: Server is listening on " << address << ":" << port << std::endl;

    while (true) {
        // Accept incoming client connections
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Server: Accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }

        std::cout << "Server: Client connected." << std::endl;

        // Handle the client
        handleClient(clientSocket);

        // After handling the client, we can break or return from the loop
        std::cout << "Server: Client disconnected or request handled. Closing connection." << std::endl;

        // Close the client socket after finishing the interaction
        closesocket(clientSocket);

        // Optionally, you can exit the loop if you want to stop the server after handling one client
        break;   // Uncomment this if you want to handle only one client and stop the server
    }

    // Clean up and close the server socket
    closesocket(serverSocket);
    WSACleanup();
}
