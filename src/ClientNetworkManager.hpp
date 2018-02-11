#ifndef CLIENT_NETWORK_MANAGER_HPP
#define CLIENT_NETWORK_MANAGER_HPP

#include <SFML/Network.hpp>
#include <iostream>

class ClientNetworkManager
{
public:
    ClientNetworkManager(sf::IpAddress& address, unsigned short& port);
    ~ClientNetworkManager();
    void connect();
    void sendData(sf::Packet& packet);
    sf::Packet receiveData();
    void disconnect(int& client_id);

private:
    sf::TcpSocket* mSocket;

    sf::Uint8 mPacketType;

    sf::IpAddress mAddress;
    unsigned short mPort;

    sf::Packet mSendPacket;
    sf::Packet mReceivePacket;
};

#endif /* CLIENT_NETWORK_MANAGER_HPP */