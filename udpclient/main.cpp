#include <iostream>
#include <chrono>

#include "UdpClient.h"

int main()
{

    UdpClient udpClien("127.0.0.1", "6000");

    while (true) {
        udpClien.Send("Hello from UDPClient");
        if (udpClien.HasMessages())
            std::cout << udpClien.PopMessage() << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

}
