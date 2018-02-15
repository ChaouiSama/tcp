#include "ServerManager.hpp"

int main(int argc, char* argv[])
{
    unsigned short port(49300);

    ServerManager server(port);

    server.run();

    while (true)
    {
        server.handleReceive();
    }

    return 0;
}