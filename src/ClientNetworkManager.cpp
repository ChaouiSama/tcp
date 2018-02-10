#include "ClientNetworkManager.hpp"

ClientNetworkManager::ClientNetworkManager()
{
    mSocket->setBlocking(true);
}

ClientNetworkManager::~ClientNetworkManager()
{
    delete mSocket;
}

void ClientNetworkManager::connect(sf::IpAddress& address, unsigned short& port)
{
    if (mSocket->connect(address, port) == sf::Socket::Done)
    {
        std::cout << "connected to server " << address << ":" << port << std::endl;
    }
    else
    {
        std::cout << "unable to connect to server " << address << ":" << port << std::endl;
    }
}

void ClientNetworkManager::sendData(sf::Packet& packet)
{
    if (mSocket->send(packet) == sf::Socket::Done)
    {
        packet.clear();
        std::cout << "packet successfully sent" << std::endl;
    }
}

sf::Packet ClientNetworkManager::receiveData()
{
    mReceivePacket.clear();

    if (mSocket->receive(mReceivePacket) == sf::Socket::Done)
    {
        std::cout << "packet successfully received" << std::endl;
        return mReceivePacket;
    }
}

void ClientNetworkManager::disconnect(int& client_id)
{
    mPacketType = 2;

    mSendPacket << mPacketType << client_id;

    sendData(mSendPacket);
}