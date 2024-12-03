#include <iostream>
#include <thread>
#include "Server.h"
#include "Client.h" 

void runServer() {
    const std::string serverAddress = "127.0.0.1";
    const int serverPort = 8080;

    std::cout << "Starting server..." << std::endl;
    startServer(serverAddress, serverPort);
    std::cout << "Server stopped." << std::endl;
}

void runClient() {
    const std::string serverAddress = "127.0.0.1";
    const int serverPort = 8080;
    const size_t arraySize = 100;
    const int numThreads = 4;
    const int range = 100;

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Allow server to start first
    std::cout << "Main: Starting client..." << std::endl;
    connectToServer(serverAddress, serverPort, arraySize, numThreads, range);
    std::cout << "Main: Client finished." << std::endl;
}

int main() {
    std::thread serverThread(runServer); // Run server in a separate thread
    std::thread clientThread(runClient); // Run client in a separate thread

    serverThread.join(); // Wait for server to finish
    clientThread.join(); // Wait for client to finish

    return 0;
}
