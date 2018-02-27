#include "Player.hpp"
#include "Graphics.hpp"
#include <cmath>

int main(int argc, char* argv[])
{
    const int WIDTH(762), HEIGHT(762);
    
    sf::IpAddress address("chaouisama.tk");
    unsigned short port(49300);

    int my_client_id, client_id;

    bool first_connection(true);

    sf::Packet send_packet, receive_packet;

    sf::Uint8 packet_type;
    sf::Uint8 action_type;

    Player player;

    float x;
    float y;

    sf::Vector2f pos;

    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, sf::VideoMode::getDesktopMode().bitsPerPixel), "test", sf::Style::Close, sf::ContextSettings(0, 0, 8, 1, 1, false));
    window->setFramerateLimit(60);

    Graphics graphics;

    graphics.loadTexture("data/grid.png");

    for (int i = 0; i <= 9; ++i)
    {
        graphics.makeSprite(i);
    }

    sf::TcpSocket socket;
    socket.setBlocking(true);

    if (socket.connect(address, port) == sf::Socket::Done)
    {
        std::cout << "connected" << std::endl;
        socket.setBlocking(false);
    }
    else
    {
        std::cout << "unable to connect" << std::endl;
    }
    
    while (window->isOpen())
    {
        sf::Event event;

        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                socket.disconnect();
                window->close();
                break;
                
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    socket.disconnect();
                    window->close();
                }
                break;

            default:
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && window->hasFocus())
        {
            player.move(my_client_id, 0.0f, -10.0f);
            packet_type = 0;
            action_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << action_type << my_client_id << pos.x << pos.y;
            if (socket.send(send_packet) != sf::Socket::Done){}
            send_packet.clear();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && window->hasFocus())
        {
            player.move(my_client_id, -10.0f, 0.0f);
            packet_type = 0;
            action_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << action_type << my_client_id << pos.x << pos.y;
            if (socket.send(send_packet) != sf::Socket::Done){}
            send_packet.clear();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && window->hasFocus())
        {
            player.move(my_client_id, 0.0f, 10.0f);
            packet_type = 0;
            action_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << action_type << my_client_id << pos.x << pos.y;
            if (socket.send(send_packet) != sf::Socket::Done){}
            send_packet.clear();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && window->hasFocus())
        {
            player.move(my_client_id, 10.0f, 0.0f);
            packet_type = 0;
            action_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << action_type << my_client_id << pos.x << pos.y;
            if (socket.send(send_packet) != sf::Socket::Done){}
            send_packet.clear();
        }

        if (socket.receive(receive_packet) == sf::Socket::Done)
        {
            receive_packet >> packet_type;

            switch (packet_type)
            {
            case 0:
                if (first_connection)
                {
                    first_connection = false;
                    receive_packet >> my_client_id;
                    receive_packet.clear();
                    std::cout << "my_client_id: " << my_client_id << std::endl;
                    player.addPlayer(my_client_id);
                }
                else
                {
                    receive_packet >> client_id;
                    receive_packet.clear();
                    std::cout << "client_id: " << client_id << std::endl;
                    player.addPlayer(client_id);
                }
                break;

            case 1:
                receive_packet >> action_type >> client_id >> x >> y;
                receive_packet.clear();
                switch (action_type)
                {
                case 0:
                    player.setPosition(client_id, x, y);
                    break;

                case 1:
                    graphics.makeHitSprite((int)x + 10, (int)y + 10);
                    break;

                default:
                    break;
                }
                break;

            case 2:
                receive_packet >> client_id;
                receive_packet.clear();
                std::cout << "deconnection packet received from client_id[" << client_id << "]" << std::endl;
                player.getPlayerList()->erase(player.getPlayerList()->find(client_id));
                break;

            default:
                break;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && window->hasFocus())
        {
            int x = (sf::Mouse::getPosition(*window).x / 38) + 1;
            int y = (sf::Mouse::getPosition(*window).y / 38) + 1;
            if (x <= 10 && y <= 10)
                graphics.makeHitSprite(x, y);

            packet_type = 0;
            action_type = 1;
            send_packet << packet_type << action_type << my_client_id << (float)x << (float)y;
            if (socket.send(send_packet) != sf::Socket::Done){}
            send_packet.clear();
        }

        window->clear(sf::Color::White);
        graphics.draw(window);
        for (std::map<int, sf::CircleShape>::iterator iter(player.getPlayerList()->begin()); iter != player.getPlayerList()->end(); ++iter)
        {
            window->draw(iter->second);
            //std::cout << "map: " << iter->first << std::endl;
        } 
        window->display();
    }

    delete window;

    return 0;
}