#include "ServerManager.hpp"

int main(int argc, char* argv[])
{
    unsigned short port(3000);

    ServerManager server(port);

    server.run();

    while (true)
    {
        server.handleReceive();
    }

    return 0;
}