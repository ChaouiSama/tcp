#include "ClientNetworkManager.hpp"

ClientNetworkManager::ClientNetworkManager()
{
    mSocket = new sf::TcpSocket;
    mSocket->setBlocking(true);
}

ClientNetworkManager::~ClientNetworkManager()
{
    delete mSocket;
}

void ClientNetworkManager::connect(sf::IpAddress& address, unsigned short& port)
{
    /*mAddress = address;
    mPort = port;*/

    if (mSocket->connect(address, port) == sf::Socket::Done)
    {
        std::cout << "connected to server " << address << ":" << port << std::endl;
        mSocket->setBlocking(false);
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

bool ClientNetworkManager::receiveData(sf::Packet* packet)
{
    packet->clear();

    if (mSocket->receive(*packet) == sf::Socket::Done)
    {
        std::cout << "packet successfully received" << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}

void ClientNetworkManager::disconnect(/*int& client_id*/)
{
    /*mPacketType = 2;

    mSendPacket << mPacketType << client_id;

    sendData(mSendPacket);*/
    mSocket->disconnect();
}