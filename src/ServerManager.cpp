#include "ServerManager.hpp"

ServerManager::ServerManager(unsigned short &port)
{
    m_port = port;
    NEXT_AVAILABLE_ID = 0;
    m_connected_clients = 0;
    m_clients_ready = 0;
    m_listener.setBlocking(false);
}

ServerManager::~ServerManager()
{
    delete m_socket;
    delete m_grid;
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

        switch ((e_packets_types)m_packet_type)
        {
        case PT_DATA_TRANSFERT:
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
    m_connected_clients++;

    if (m_connected_clients > 2)
    {
        m_packet_type = PT_CLIENT_DISCONNECTION;
        m_send_packet << m_packet_type << NEXT_AVAILABLE_ID;
        m_socket->send(m_send_packet);
        m_send_packet.clear();
        NEXT_AVAILABLE_ID--;
        m_connected_clients--;
        m_selector.remove(*m_socket);
        m_socket->disconnect();
        delete m_socket;
        std::cout << "a client tried to connect but the server is full" << std::endl;
    }
    else
    {
        m_packet_type = PT_CLIENT_CONNECTION;
        m_send_packet << m_packet_type << NEXT_AVAILABLE_ID;

        m_clients.emplace(NEXT_AVAILABLE_ID, m_socket);
        m_selector.add(*m_socket);
        sendToAll(m_send_packet);
        m_receive_packet.clear();

        for (std::map<int, sf::TcpSocket*>::iterator iter(m_clients.begin()); iter != m_clients.end(); ++iter)
        {
            if (iter->first != NEXT_AVAILABLE_ID)
            {
                m_packet_type = PT_CLIENT_CONNECTION;
                m_send_packet << m_packet_type << iter->first;
                sendTo(m_send_packet, NEXT_AVAILABLE_ID);
            }
        }
    }
}

void ServerManager::handleDataTransfert()
{
    m_receive_packet >> m_action_type;

    switch ((e_packets_types)m_action_type)
    {
    case PT_SHIPS_PLACEMENT:
        m_receive_packet >> m_client_id;
        m_grid = new std::vector<std::vector<int>>(10, std::vector<int>(10));
        m_receive_packet >> *m_grid;
        m_grids.emplace(m_client_id, *m_grid);
        m_clients_ready++;
        if (m_clients_ready == 2)
        {
            m_packet_type = PT_DATA_TRANSFERT;
            m_action_type = PT_NEW_TURN;
            m_send_packet << m_packet_type << m_action_type << m_clients.begin()->first;
            sendToAll(m_send_packet);
        }
        m_receive_packet.clear();
        break;

    case PT_HIT_PLACEMENT:
        m_receive_packet >> m_client_id >> m_x >> m_y;
        for (auto iter(m_grids.begin()); iter != m_grids.end(); ++iter)
        {
            if (iter->first != m_client_id)
            {
                m_receive_packet.clear();
                iter->second.at(m_y).at(m_x) = 2;
                m_packet_type = PT_DATA_TRANSFERT;
                m_action_type = PT_HIT_PLACEMENT;
                m_send_packet << m_packet_type << m_action_type << m_x << m_y;
                int id = iter->first;
                sendTo(m_send_packet, id);
                m_action_type = PT_NEW_TURN;
                m_send_packet << m_packet_type << m_action_type << id;
                sendToAll(m_send_packet);
            }
        }
        break;

    default:
        break;
    }
}

void ServerManager::handleDisconnection()
{
    m_mutex.lock();
    for (std::map<int, sf::TcpSocket*>::iterator iter(m_clients.begin()); iter != m_clients.end(); ++iter)
    {
        if (iter->second == m_socket)
        {
            m_packet_type = PT_CLIENT_DISCONNECTION;
            m_send_packet << m_packet_type << iter->first;

            std::cout << "client id[" << iter->first << "] has disconnected" << std::endl;
            
            m_selector.remove(*iter->second);
            iter->second->disconnect();
            delete iter->second;
            m_clients.erase(iter);
            if (!m_grids.empty())
            {
                m_grids.erase(m_grids.find(iter->first));
            }
            break;
        }
    }
    m_mutex.unlock();
    m_connected_clients--;
    m_clients_ready--;
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

sf::Packet& operator <<(sf::Packet &packet, std::vector<std::vector<int>> &data)
{
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
}