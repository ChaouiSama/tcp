#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

#include <SFML/Network.hpp>
#include <iostream>

class ServerManager
{
public:
    ServerManager(int port);
    ~ServerManager();

    void run();
    void handleReceive();
    void receiveAndGetPacketType();
    void handleConnection();
    void handleDataTransfert();
    void handleDisconnection();

    void sendToAll(sf::Packet packet);
    void sendToAllExcept(sf::Packet packet, int client_id);

private:
    sf::TcpSocket* mSocket;
    sf::TcpListener mListener;
    sf::SocketSelector mSelector;

    int mPort;
    int NEXT_AVAILABLE_ID;
    int mClientId;

    float mX;
    float mY;

    sf::Uint8 mPacketType;

    sf::Packet mReceivePacket;
    sf::Packet mSendPacket;

    std::map<int, sf::TcpSocket*> mClients;
};

#endif /* SERVER_MANAGER_HPP */