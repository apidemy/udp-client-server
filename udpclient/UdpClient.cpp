#include <iostream>

#include "UdpClient.h"

UdpClient::UdpClient(string udpSocketIP, string udpPort)
        :
        resolver(io_service),
        query(udp::v4(), udpSocketIP, udpPort),
        socket(io_service),
        service_thread(&UdpClient::run_service, this)
{
    remote_endpoint = *resolver.resolve(query);
    socket.open(udp::v4());
}

UdpClient::~UdpClient()
{
    io_service.stop();
    service_thread.join();
}

void UdpClient::start_receive()
{
    socket.async_receive_from(asio::buffer(recv_buffer), remote_endpoint,
            [this](std::error_code ec, std::size_t bytes_recvd) {
              this->handle_receive(ec,
                      bytes_recvd);
            });
}

void UdpClient::handle_receive(const std::error_code& error, std::size_t bytes_transferred)
{
    if (!error) {
        std::string message(recv_buffer.data(), recv_buffer.data()+bytes_transferred);
        incomingMessages.push(message);
    }

    start_receive();
}

void UdpClient::Send(std::string message)
{
    socket.send_to(asio::buffer(message), remote_endpoint);

}

bool UdpClient::HasMessages()
{
    return !incomingMessages.empty();
}

std::string UdpClient::PopMessage()
{
    if (incomingMessages.empty())
        throw std::logic_error("No messages to pop");
    return incomingMessages.pop();
}

void UdpClient::run_service()
{
    start_receive();
    while (!io_service.stopped()) {
        try {
            io_service.run();
        }
        catch (const std::exception& e) {
            std::cout << "Client: network exception: " << e.what() << std::endl;
        }
        catch (...) {
            std::cout << "Unknown exception in client network thread\n";
        }
    }
}