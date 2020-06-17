#include <iostream>
#include <chrono>
#include "UDPServer.h"

using namespace NetworkLib;

int main()
{

    /*
     * Example of how to use UDPServer Class
     */

    auto server = new UDPServer(6000);

    while (true) {
        if (server->HasMessages()) {
            ClientMessage clientMessage = server->PopMessage();
            std::cout << "Client Msg: " << clientMessage.first << " ClientID: " << clientMessage.second << "\n";
            server->SendToClient("Hello back to client", clientMessage.second);

            // Show network statistics
            std::cout << "Statistics: " << server->GetStatistics() << "\n";
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }

    return 0;
}