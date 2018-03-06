#include "ServerManager.hpp"

ServerManager::ServerManager(unsigned short &port)
{
    m_port = port;
    NEXT_AVAILABLE_ID = 0;
    m_listener.setBlocking(false);
}

ServerManager::~ServerManager()
{
    delete m_socket;
}

void ServerManager::run()
{
    if (m_listener.listen(m_port) != sf::TcpListener::Done)
    {
        std::cout << "Listener unable to listen on port: " << m_port << std::endl;
        //exit(-1);
    }
    std::cout << "Listener listening on port: " << m_port << std::endl;
    m_selector.add(m_listener);
}

void ServerManager::handleReceive()
{
    if (m_selector.wait())
    {
        if (m_selector.isReady(m_listener))
        {
            m_socket = new sf::TcpSocket;
            
            if (m_listener.accept(*m_socket) == sf::TcpListener::Done)
            {
                std::cout << "client accepted" << std::endl;
                handleConnection();
            }
        }
        else
        {
            for (std::map<int, sf::TcpSocket*>::iterator iter(m_clients.begin()); iter != m_clients.end(); ++iter)
            {
                if (m_selector.isReady(*iter->second))
                {
                    m_socket = iter->second;
                    receiveAndGetPacketType();
                    break;
                }
            }
        }
    }
}

void ServerManager::receiveAndGetPacketType()
{
    if (m_socket->receive(m_receive_packet) == sf::Socket::Done)
    {
        m_receive_packet >> m_packet_type;

        switch (m_packet_type)
        {
        case 0:
            handleDataTransfert();
            break;

        default:
            break;
        };
    }
    else if (m_socket->receive(m_receive_packet) == sf::Socket::Disconnected)
    {
        handleDisconnection();
    }
}

void ServerManager::handleConnection()
{
    NEXT_AVAILABLE_ID += 1;

    m_packet_type = 0;
    m_send_packet << m_packet_type << NEXT_AVAILABLE_ID;

    m_clients.emplace(NEXT_AVAILABLE_ID, m_socket);
    m_selector.add(*m_socket);
    sendToAll(m_send_packet);
    m_receive_packet.clear();

    for (std::map<int, sf::TcpSocket*>::iterator iter(m_clients.begin()); iter != m_clients.end(); ++iter)
    {
        if (iter->first != NEXT_AVAILABLE_ID)
        {
            m_packet_type = 0;
            m_send_packet << m_packet_type << iter->first;
            sendTo(m_send_packet, NEXT_AVAILABLE_ID);
        }
    }
}

void ServerManager::handleDataTransfert()
{
    m_receive_packet >> m_action_type >> m_client_id >> m_x >> m_y;

    m_packet_type = 1;
    m_send_packet << m_packet_type << m_action_type << m_client_id << m_x << m_y;
    sendToAllExcept(m_send_packet, m_client_id);
}

void ServerManager::handleDisconnection()
{
    m_mutex.lock();
    for (std::map<int, sf::TcpSocket*>::iterator iter(m_clients.begin()); iter != m_clients.end(); ++iter)
    {
        if (iter->second == m_socket)
        {
            m_packet_type = 2;
            m_send_packet << m_packet_type << iter->first;

            std::cout << "client id[" << iter->first << "] has disconnected" << std::endl;
            
            m_selector.remove(*iter->second);
            iter->second->disconnect();
            delete iter->second;
            m_clients.erase(iter);
            break;
        }
    }
    m_mutex.unlock();
    sendToAll(m_send_packet);
}

void ServerManager::sendToAll(sf::Packet &packet)
{
    for (std::map<int, sf::TcpSocket*>::iterator iter(m_clients.begin()); iter != m_clients.end(); ++iter)
    {
        if (iter->second->send(packet) != sf::Socket::Done)
            std::cout << "unable to send packet to id[" << iter->first << "]" << std::endl;
    }

    packet.clear();
}

void ServerManager::sendToAllExcept(sf::Packet &packet, int &client_id)
{
    for (std::map<int, sf::TcpSocket*>::iterator iter(m_clients.begin()); iter != m_clients.end(); ++iter)
    {
        if (iter->first != client_id)
        {
            if (iter->second->send(packet) != sf::Socket::Done)
                std::cout << "unable to send packet to id[" << iter->first << "]" << std::endl;
        }
    }

    packet.clear();
}

void ServerManager::sendTo(sf::Packet &packet, int &client_id)
{
    if (m_clients.find(client_id)->second->send(packet) != sf::Socket::Done)
        std::cout << "unable to send packet to id[" << client_id << "]" << std::endl;

    packet.clear();
}