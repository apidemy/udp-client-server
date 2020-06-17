#ifndef NETWORKLIB_UDPCLIENT
#define NETWORKLIB_UDPCLIENT

#include <string>
#include <boost/asio.hpp>
#include <thread>
#include <array>

#include "LockedQueue.h"

using namespace std;
using boost::asio::ip::udp;
using namespace boost;

static const int NetworkBufferSize = 4096;

class UdpClient {
private:
    // Network send/receive stuff
    asio::io_service io_service;
    udp::resolver resolver;
    udp::resolver::query query;
    udp::socket socket;
    udp::endpoint remote_endpoint;
    std::array<char, NetworkBufferSize> recv_buffer;
    std::thread service_thread;

    // Queues for messages
    NetworkLib::LockedQueue<std::string> incomingMessages;

public:
    UdpClient(string udpSocketIP, string udpPort);
    ~UdpClient();
    void Send(std::string messasge);
    bool HasMessages();
    std::string PopMessage();

private:
    void start_receive();
    void handle_receive(const std::error_code& error, std::size_t bytes_transferred);
    void run_service();

    UdpClient(UdpClient&); // block default copy constructor
};

#endif //NETWORKLIB_UDPCLIENT
