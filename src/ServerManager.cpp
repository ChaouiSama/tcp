#include "ServerManager.hpp"

ServerManager::ServerManager(unsigned short port)
{
    mPort = port;
    NEXT_AVAILABLE_ID = 0;
    mListener.setBlocking(false);
}

ServerManager::~ServerManager()
{
    delete mSocket;
}

void ServerManager::run()
{
    if (mListener.listen(mPort) != sf::TcpListener::Done)
    {
        std::cout << "Listener unable to listen on port: " << mPort << std::endl;
        //exit(-1);
    }
    std::cout << "Listener listening on port: " << mPort << std::endl;
    mSelector.add(mListener);
}

void ServerManager::handleReceive()
{
    if (mSelector.wait())
    {
        if (mSelector.isReady(mListener))
        {
            if (mListener.accept(*mSocket) == sf::Socket::Done)
            {
                receiveAndGetPacketType();
            }
        }
        else
        {
            for (std::map<int, sf::TcpSocket*>::iterator iter(mClients.begin()); iter != mClients.end(); ++iter)
            {
                if (mSelector.isReady(*iter->second))
                {
                    receiveAndGetPacketType();
                }
            }
        }
    }
}

void ServerManager::receiveAndGetPacketType()
{
    if (mSocket->receive(mReceivePacket) == sf::Socket::Done)
    {
        mReceivePacket >> mPacketType;

        switch (mPacketType)
        {
        case 0:
            handleConnection();
            break;

        case 1:
            handleDataTransfert();
            break;

        case 2:
            handleDisconnection();
            break;

        default:
            break;
        };
    }
}

void ServerManager::handleConnection()
{
    NEXT_AVAILABLE_ID += 1;

    mPacketType = 0;
    mSendPacket << mPacketType << NEXT_AVAILABLE_ID;
    sendToAll(mSendPacket);

    mClients.insert(std::pair<int, sf::TcpSocket*>(NEXT_AVAILABLE_ID, mSocket));
    mSelector.add(*mSocket);
    mReceivePacket.clear();
}

void ServerManager::handleDataTransfert()
{
    mReceivePacket >> mClientId >> mX >> mY;

    mPacketType = 1;
    mSendPacket << mPacketType << mClientId << mX << mY;
    sendToAllExcept(mSendPacket, mClientId);
}

void ServerManager::handleDisconnection()
{
    mReceivePacket >> mClientId;

    for (std::map<int, sf::TcpSocket*>::iterator iter(mClients.begin()); iter != mClients.end(); ++iter)
    {
        if (iter->first == mClientId)
        {
            mClients.erase(iter);
        }

        mPacketType = 2;
        mSendPacket << mPacketType << mClientId;
        sendToAll(mSendPacket);
    }

    mReceivePacket.clear();
}

void ServerManager::sendToAll(sf::Packet packet)
{
    for (std::map<int, sf::TcpSocket*>::iterator iter(mClients.begin()); iter != mClients.end(); ++iter)
    {
        if (iter->second->send(packet) != sf::Socket::Done)
            std::cout << "unable to send packet to id[" << iter->first << "]" << std::endl;
    }

    mSendPacket.clear();
}

void ServerManager::sendToAllExcept(sf::Packet packet, int client_id)
{
    for (std::map<int, sf::TcpSocket*>::iterator iter(mClients.begin()); iter != mClients.end(); ++iter)
    {
        if (iter->first != client_id)
        {
            if (iter->second->send(packet) != sf::Socket::Done)
                std::cout << "unable to send packet to id[" << iter->first << "]" << std::endl;
        }
    }

    mSendPacket.clear();
}