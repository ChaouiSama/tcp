#ifndef CLIENT_MANAGER_HPP
#define CLIENT_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "ClientNetworkManager.hpp"
#include "Graphics.hpp"
#include <iostream>

class ClientManager
{
public:
    ClientManager();

    void addClient(int client_id);
    std::vector<int> *getClientList();

    bool shipsOverlapping(std::vector<std::vector<int>> *board, int *x, int *y, int *dir, int *length);
    bool shipsAdjoined(std::vector<std::vector<int>> *board, int *x, int *y, int *dir, int *length);
    
private:
    std::vector<int> m_clients;
};

#endif /* CLIENT_MANAGER_HPP */