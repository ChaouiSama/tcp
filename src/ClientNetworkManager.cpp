#include "ClientNetworkManager.hpp"

ClientNetworkManager::ClientNetworkManager()
{
    m_socket = new sf::TcpSocket;
    m_socket->setBlocking(true);
}

ClientNetworkManager::~ClientNetworkManager()
{
    delete m_socket;
}

void ClientNetworkManager::connect(sf::IpAddress &address, unsigned short &port)
{
    if (m_socket->connect(address, port) == sf::Socket::Done)
    {
        std::cout << "connected to server " << address << ":" << port << std::endl;
        m_socket->setBlocking(false);
    }
    else
    {
        std::cout << "unable to connect to server " << address << ":" << port << std::endl;
    }
}

void ClientNetworkManager::sendData(sf::Packet &packet)
{
    if (m_socket->send(packet) == sf::Socket::Done)
    {
        packet.clear();
        //std::cout << "packet successfully sent" << std::endl;
    }
}

sf::Packet ClientNetworkManager::receiveData()
{
    m_receive_packet.clear();

    if (m_socket->receive(m_receive_packet) == sf::Socket::Done)
    {
        //std::cout << "packet successfully received" << std::endl;
        return m_receive_packet;
    }
}

bool ClientNetworkManager::receiveData(sf::Packet *packet)
{
    packet->clear();

    if (m_socket->receive(*packet) == sf::Socket::Done)
    {
        //std::cout << "packet successfully received" << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}

void ClientNetworkManager::disconnect()
{
    m_socket->disconnect();
}

/*sf::Packet& operator <<(sf::Packet &packet, std::vector<std::vector<int>> &data)
{
    packet << data.size();
    packet << data.at(0).size();

    for (int i(0); i < data.size(); ++i)
    {
        for (int j(0); j < data.at(i).size(); ++j)
        {
            packet << data.at(i).at(j);
        }
    }

    return packet;
}

sf::Packet& operator >>(sf::Packet &packet, std::vector<std::vector<int>> &data)
{
    int row_size;
    int col_size;

    packet >> col_size;
    packet >> row_size;

    data.resize(col_size, std::vector<int>(row_size, 0));

    for (int i(0); i < data.size(); ++i)
    {
        for (int j(0); j < data.at(i).size(); ++j)
        {
            int temp;
            packet >> temp;
            data.at(i).at(j) = temp;
        }
    }

    return packet;
}*/