#include "UDPServer.h"
#include <iostream>

namespace NetworkLib {
UDPServer::UDPServer(unsigned short local_port)
        :
        socket(io_service, udp::endpoint(udp::v4(), local_port)),
        service_thread(&UDPServer::run_service, this),
        nextClientID(0L)
{
    std::cout << "Starting server on port ", local_port;
};

UDPServer::~UDPServer()
{
    io_service.stop();
    service_thread.join();
}

void UDPServer::start_receive()
{
    socket.async_receive_from(asio::buffer(recv_buffer), remote_endpoint,
            [this](std::error_code ec, std::size_t bytes_recvd) { this->handle_receive(ec, bytes_recvd); });
}

void UDPServer::on_client_disconnected(int32_t id)
{
    for (auto& handler : clientDisconnectedHandlers)
        if (handler)
            handler(id);
}

void UDPServer::handle_remote_error(const std::error_code error_code, const udp::endpoint remote_endpoint)
{
    bool found = false;
    int32_t id;
    for (const auto& client : clients)
        if (client.second==remote_endpoint) {
            found = true;
            id = client.first;
            break;
        }
    if (found==false)
        return;

    clients.erase(id);
    on_client_disconnected(id);
}

void UDPServer::handle_receive(const std::error_code& error, std::size_t bytes_transferred)
{
    if (!error) {
        try {
            auto message = ClientMessage(std::string(recv_buffer.data(), recv_buffer.data()+bytes_transferred),
                    get_or_create_client_id(remote_endpoint));
            if (!message.first.empty())
                incomingMessages.push(message);
            statistics.RegisterReceivedMessage(bytes_transferred);
        }
        catch (std::exception ex) {
//				Log::Error("handle_receive: Error parsing incoming message:", ex.what());
        }
        catch (...) {
//				Log::Error("handle_receive: Unknown error while parsing incoming message");
        }
    }
    else {
//			Log::Error("handle_receive: error: ", error.message(), " while receiving from address ", remote_endpoint);
        handle_remote_error(error, remote_endpoint);
    }

    start_receive();
}

void UDPServer::send(const std::string& message, udp::endpoint target_endpoint)
{
    socket.send_to(asio::buffer(message), target_endpoint);
    statistics.RegisterSentMessage(message.size());
}

void UDPServer::run_service()
{
    start_receive();
    while (!io_service.stopped()) {
        try {
            io_service.run();
        }
        catch (const std::exception& e) {
//				Log::Error("UDPServer: Network exception: ", e.what());
        }
        catch (...) {
            //	Log::Error("UDPServer: Network exception: unknown");
        }
    }
    //Log::Debug("UDPServer network thread stopped");
};

int32_t UDPServer::get_or_create_client_id(udp::endpoint endpoint)
{
    for (const auto& client : clients)
        if (client.second==endpoint)
            return client.first;

    auto id = ++nextClientID;
    clients.insert(Client(id, endpoint));
    return id;
};

void UDPServer::SendToClient(const std::string& message, uint32_t clientID)
{
    try {
        send(message, clients.at(clientID));
    }
    catch (std::out_of_range) {
        //Log::Error(__FUNCTION__": Unknown client ID ", clientID);
    }
};

void UDPServer::SendToAllExcept(const std::string& message, uint32_t clientID)
{
    for (auto client : clients)
        if (client.first!=clientID)
            send(message, client.second);
};

void UDPServer::SendToAll(const std::string& message)
{
    for (auto client : clients)
        send(message, client.second);
}

size_t UDPServer::GetClientCount()
{
    return clients.size();
}

uint32_t UDPServer::GetClientIdByIndex(size_t index)
{
    auto it = clients.begin();
    for (int i = 0; i<index; i++)
        ++it;
    return it->first;
};

ClientMessage UDPServer::PopMessage()
{
    return incomingMessages.pop();
}

bool UDPServer::HasMessages()
{
    return !incomingMessages.empty();
};
}