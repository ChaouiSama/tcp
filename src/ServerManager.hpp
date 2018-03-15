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
    int m_connected_clients;
    int m_clients_ready;

    float m_x;
    float m_y;

    sf::Packet m_receive_packet;
    sf::Packet m_send_packet;

    sf::Mutex m_mutex;

    std::map<int, sf::TcpSocket*> m_clients;

    enum e_packets_types : sf::Uint8
    {
        PT_CLIENT_CONNECTION,
        PT_DATA_TRANSFERT,
        PT_CLIENT_DISCONNECTION,
        PT_SHIPS_PLACEMENT,
        PT_HIT_PLACEMENT,
        PT_NEW_TURN
    };
    
    sf::Uint8 m_packet_type;
    sf::Uint8 m_action_type;

    std::vector<std::vector<int>> *m_grid;
    std::map<int, std::vector<std::vector<int>>> m_grids; 
};

sf::Packet& operator <<(sf::Packet &packet, std::vector<std::vector<int>> &data);
sf::Packet& operator >>(sf::Packet &packet, std::vector<std::vector<int>> &data);

#endif /* SERVER_MANAGER_HPP_ */