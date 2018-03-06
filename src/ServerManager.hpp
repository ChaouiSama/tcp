#ifndef SERVER_MANAGER_HPP_
#define SERVER_MANAGER_HPP_

#include <SFML/Network.hpp>
#include <iostream>
#include <memory>

class ServerManager
{
public:
    ServerManager(unsigned short& port);
    ~ServerManager();

    void run();
    void handleReceive();
    void receiveAndGetPacketType();
    void handleConnection();
    void handleDataTransfert();
    void handleDisconnection();

    void sendToAll(sf::Packet &packet);
    void sendToAllExcept(sf::Packet &packet, int &client_id);
    void sendTo(sf::Packet &packet, int &client_id);

private:
    sf::TcpSocket *m_socket;
    sf::TcpListener m_listener;
    sf::SocketSelector m_selector;

    unsigned short m_port;
    int NEXT_AVAILABLE_ID;
    int m_client_id;

    float m_x;
    float m_y;

    sf::Uint8 m_packet_type;
    sf::Uint8 m_action_type;

    sf::Packet m_receive_packet;
    sf::Packet m_send_packet;

    sf::Mutex m_mutex;

    std::map<int, sf::TcpSocket*> m_clients;
};

#endif /* SERVER_MANAGER_HPP_ */