#ifndef SERVER_MANAGER_HPP
#define SERVER_MANAGER_HPP

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

    void sendToAll(sf::Packet& packet);
    void sendToAllExcept(sf::Packet& packet, int& client_id);
    void sendTo(sf::Packet& packet, int& client_id);

private:
    sf::TcpSocket* mSocket;
    sf::TcpListener mListener;
    sf::SocketSelector mSelector;

    unsigned short mPort;
    int NEXT_AVAILABLE_ID;
    int mClientId;

    float mX;
    float mY;

    sf::Uint8 mPacketType;

    sf::Packet mReceivePacket;
    sf::Packet mSendPacket;

    sf::Mutex mMutex;

    std::map<int, sf::TcpSocket*> mClients;
};

#endif /* SERVER_MANAGER_HPP */