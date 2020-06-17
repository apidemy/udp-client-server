#ifndef NETWORKLIB_UDPSERVER
#define NETWORKLIB_UDPSERVER
#include "Statistics.h"
#include "LockedQueue.h"

#include <boost/asio.hpp>
#include <array>
#include <map>
#include <thread>
#include <atomic>

using namespace boost;
using asio::ip::udp;

typedef std::map<uint32_t, udp::endpoint> ClientList;
typedef ClientList::value_type Client;
typedef std::pair<std::string, uint32_t> ClientMessage;

static const int NetworkBufferSize = 4096;

namespace NetworkLib {
class UDPServer {
public:
    explicit UDPServer(unsigned short local_port);
    virtual ~UDPServer();

    bool HasMessages();
    ClientMessage PopMessage();

    void SendToClient(const std::string& message, uint32_t clientID);
    void SendToAllExcept(const std::string& message, uint32_t clientID);
    void SendToAll(const std::string& message);

    size_t GetClientCount();
    uint32_t GetClientIdByIndex(size_t index);

    const Statistics& GetStatistics() const { return statistics; };
    std::vector<std::function<void(uint32_t)>> clientDisconnectedHandlers;
private:
    // Network send/receive stuff
    asio::io_service io_service;
    udp::socket socket;
    udp::endpoint server_endpoint;
    udp::endpoint remote_endpoint;
    std::array<char, NetworkBufferSize> recv_buffer;
    std::thread service_thread;

    // Low-level network functions
    void start_receive();
    void handle_remote_error(std::error_code error_code, udp::endpoint remote_endpoint);
    void handle_receive(const std::error_code& error, std::size_t bytes_transferred);
//		void handle_send(std::string /*message*/, const std::error_code& /*error*/, std::size_t /*bytes_transferred*/)	{}
    void run_service();

    // Client management
    int32_t get_or_create_client_id(udp::endpoint endpoint);
    void on_client_disconnected(int32_t id);

    void send(const std::string& message, udp::endpoint target);

    // Incoming messages queue
    LockedQueue<ClientMessage> incomingMessages;

    // Clients of the server
    ClientList clients;
    std::atomic_int32_t nextClientID;

    UDPServer(UDPServer&); // block default copy constructor

    // Statistics
    Statistics statistics;
};
}
#endif // NETWORKLIB_UDPSERVER