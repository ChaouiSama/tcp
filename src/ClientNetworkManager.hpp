#ifndef CLIENT_NETWORK_MANAGER_HPP_
#define CLIENT_NETWORK_MANAGER_HPP_

#include <SFML/Network.hpp>
#include <iostream>

class ClientNetworkManager
{
public:
    ClientNetworkManager();
    ~ClientNetworkManager();
    void connect(sf::IpAddress &address, unsigned short &port);
    void sendData(sf::Packet &packet);
    sf::Packet receiveData();
    bool receiveData(sf::Packet *packet);
    void disconnect();

private:
    sf::TcpSocket *m_socket;

    sf::Uint8 m_packet_type;

    sf::Packet m_send_packet;
    sf::Packet m_receive_packet;
};
    
/*sf::Packet& operator <<(sf::Packet &packet, std::vector<std::vector<int>> &data);
sf::Packet& operator >>(sf::Packet &packet, std::vector<std::vector<int>> &data);*/

#endif /* CLIENT_NETWORK_MANAGER_HPP_ */
