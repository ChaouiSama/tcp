#include "ServerManager.hpp"

ServerManager::ServerManager(unsigned short& port)
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
            mSocket = new sf::TcpSocket;
            
            if (mListener.accept(*mSocket) == sf::TcpListener::Done)
            {
                std::cout << "accepted" << std::endl;
                handleConnection();
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
            handleDataTransfert();
            break;

        default:
            break;
        };
    }
    else if (mSocket->receive(mReceivePacket) == sf::Socket::Disconnected)
    {
        handleDisconnection();
    }
}

void ServerManager::handleConnection()
{
    NEXT_AVAILABLE_ID += 1;

    mPacketType = 0;
    mSendPacket << mPacketType << NEXT_AVAILABLE_ID;

    mClients.emplace(NEXT_AVAILABLE_ID, mSocket);
    mSelector.add(*mSocket);
    sendToAll(mSendPacket);
    mReceivePacket.clear();

    if (!mClients.empty())
    {
        for (std::map<int, sf::TcpSocket*>::iterator iter(mClients.begin()); iter != mClients.end(); ++iter)
        {
            std::cout << iter->first << std::endl;

            if (iter->first != NEXT_AVAILABLE_ID)
            {
                mPacketType = 0;
                mSendPacket << mPacketType << iter->first;
                sendTo(mSendPacket, NEXT_AVAILABLE_ID);
            }
        }
    }
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
    mMutex.lock();
    for (std::map<int, sf::TcpSocket*>::iterator iter(mClients.begin()); iter != mClients.end(); ++iter)
    {
        if (iter->second == mSocket)
        {
            mPacketType = 2;
            mSendPacket << mPacketType << iter->first;
            
            mSelector.remove(*iter->second);
            delete iter->second;
            mClients.erase(iter);

        }
    }
    mMutex.unlock();
    sendToAll(mSendPacket);
}

void ServerManager::sendToAll(sf::Packet& packet)
{
    for (std::map<int, sf::TcpSocket*>::iterator iter(mClients.begin()); iter != mClients.end(); ++iter)
    {
        if (iter->second->send(packet) != sf::Socket::Done)
            std::cout << "unable to send packet to id[" << iter->first << "]" << std::endl;
    }

    packet.clear();
}

void ServerManager::sendToAllExcept(sf::Packet& packet, int& client_id)
{
    for (std::map<int, sf::TcpSocket*>::iterator iter(mClients.begin()); iter != mClients.end(); ++iter)
    {
        if (iter->first != client_id)
        {
            if (iter->second->send(packet) != sf::Socket::Done)
                std::cout << "unable to send packet to id[" << iter->first << "]" << std::endl;
        }
    }

    packet.clear();
}

void ServerManager::sendTo(sf::Packet& packet, int& client_id)
{
    if (mClients.find(client_id)->second->send(packet) != sf::Socket::Done)
        std::cout << "unable to send packet to id[" << client_id << "]" << std::endl;

    packet.clear();
}