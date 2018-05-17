#include "ClientManager.hpp"

ClientManager::ClientManager()
{
    
}

void ClientManager::addClient(int client_id)
{
    m_clients.push_back(client_id);
}

std::vector<int>* ClientManager::getClientList()
{
    return &m_clients;
}

bool ClientManager::shipsOverlapping(std::vector<std::vector<int>> *board, int *x, int *y, int *dir, int *length)
{
    switch (*dir)
    {
    case 1:
        for (int i(*y - 1); i > (*y - 1) - (*length - 1); --i)
        {
            if (board->at(i).at(*x - 13) == 1)
                return true;
        }
        return false;
        break;

    case 2:
        for (int i(*x - 13); i > (*x - 13) - (*length - 1); --i)
        {
            if (board->at(*y - 1).at(i) == 1)
                return true;
        }
        return false;
        break;

    case 3:
        for (int i(*y - 1); i < (*y - 1) + (*length - 1); ++i)
        {
            if (board->at(i).at(*x - 13) == 1)
                return true;
        }
        return false;
        break;

    case 4:
        for (int i(*x - 13); i < (*x - 13) + (*length - 1); ++i)
        {
            if (board->at(*y - 1).at(i) == 1)
                return true;
        }
        return false;
        break;

    default:
        break;
    }
}

bool ClientManager::shipsAdjoined(std::vector<std::vector<int>> *board, int *x, int *y, int *dir, int *length)
{
    switch (*dir)
    {
    case 1:
        for (int i(*y - 1); i >= (*y - 1) - (*length - 1); --i)
        {
            if (i > 0 && board->at(i - 1).at(*x - 13) == 1)
                return true;
            else if (i < 9 && board->at(i + 1).at(*x - 13) == 1)
                return true;
            else if ((*x - 13) > 0 && board->at(i).at(*x - 14) == 1)
                return true;
            else if ((*x - 13) < 9 && board->at(i).at(*x - 12) == 1)
                return true;
            else if (i > 0 && (*x - 13) > 0 && board->at(i - 1).at(*x - 14) == 1)
                return true;
            else if (i < 9 && (*x - 13) > 0 && board->at(i + 1).at(*x - 14) == 1)
                return true;
            else if (i > 0 && (*x - 13) < 9 && board->at(i - 1).at(*x - 12) == 1)
                return true;
            else if (i < 9 && (*x - 13) < 9 && board->at(i + 1).at(*x - 12) == 1)
                return true;
        }
        return false;
        break;

    case 2:
        for (int i(*x - 13); i >= (*x - 13) - (*length - 1); --i)
        {
            if ((*y - 1) > 0 && board->at(*y - 2).at(i) == 1)
                return true;
            else if (*y < 9 && board->at(*y).at(i) == 1)
                return true;
            else if ((*x - 13) > 0 && board->at(*y - 1).at(i - 1) == 1)
                return true;
            else if ((*x - 13) < 9 && board->at(*y - 1).at(i + 1) == 1)
                return true;
            else if ((*y - 1) > 0 && i > 0 && board->at(*y - 2).at(i - 1) == 1)
                return true;
            else if (*y < 9 && i > 0 && board->at(*y).at(i - 1) == 1)
                return true;
            else if ((*y - 1) > 0 && i < 9 && board->at(*y - 2).at(i + 1) == 1)
                return true;
            else if (*y < 9 && i < 9 && board->at(*y).at(i + 1) == 1)
                return true;
        }
        return false;
        break;

    case 3:
        for (int i(*y - 1); i <= (*y - 1) + (*length - 1); ++i)
        {
            if (i > 0 && board->at(i - 1).at(*x - 13) == 1)
                return true;
            else if (i < 9 && board->at(i + 1).at(*x - 13) == 1)
                return true;
            else if ((*x - 13) > 0 && board->at(i).at(*x - 14) == 1)
                return true;
            else if ((*x - 13) < 9 && board->at(i).at(*x - 12) == 1)
                return true;
            else if (i > 0 && (*x - 13) > 0 && board->at(i - 1).at(*x - 14) == 1)
                return true;
            else if (i < 9 && (*x - 13) > 0 && board->at(i + 1).at(*x - 14) == 1)
                return true;
            else if (i > 0 && (*x - 13) < 9 && board->at(i - 1).at(*x - 12) == 1)
                return true;
            else if (i < 9 && (*x - 13) < 9 && board->at(i + 1).at(*x - 12) == 1)
                return true;
        }
        return false;
        break;

    case 4:
        for (int i(*x - 13); i <= (*x - 13) + (*length - 1); ++i)
        {
            if ((*y - 1) > 0 && board->at(*y - 2).at(i) == 1)
                return true;
            else if (*y < 9 && board->at(*y).at(i) == 1)
                return true;
            else if ((*x - 13) > 0 && board->at(*y - 1).at(i - 1) == 1)
                return true;
            else if ((*x - 13) < 9 && board->at(*y - 1).at(i + 1) == 1)
                return true;
            else if ((*y - 1) > 0 && i > 0 && board->at(*y - 2).at(i - 1) == 1)
                return true;
            else if (*y < 9 && i > 0 && board->at(*y).at(i - 1) == 1)
                return true;
            else if ((*y - 1) > 0 && i < 9 && board->at(*y - 2).at(i + 1) == 1)
                return true;
            else if (*y < 9 && i < 9 && board->at(*y).at(i + 1) == 1)
                return true;
        }
        return false;
        break;

    default:
        break;
    }
}