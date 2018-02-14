#include "Player.hpp"

#include <SFML/Graphics.hpp>
/*#include <SFML/Network.hpp>
#include <iostream>*/

int main(int argc, char* argv[])
{
    const int WIDTH(842), HEIGHT(842);
    
    sf::IpAddress address("chaouisama.tk");
    unsigned short port(3000);

    int my_client_id, client_id;

    bool first_connection(true);

    sf::Packet send_packet, receive_packet;

    sf::Uint8 packet_type;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Sprite sprite2;

    Player player;

    float x;
    float y;

    sf::Vector2f pos;

    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, sf::VideoMode::getDesktopMode().bitsPerPixel), "test", sf::Style::Close, sf::ContextSettings(0, 0, 8, 1, 1, false));
    window->setFramerateLimit(60);

    if (!texture.loadFromFile("grid.png"))
        texture.loadFromFile("./data/grid.png");
    sprite.setTexture(texture);
    sprite.setPosition(0, 0);
    sprite2.setTexture(texture);
    sprite2.setPosition(420, 420);

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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            player.move(my_client_id, 0.0f, -10.0f);
            packet_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << my_client_id << pos.x << pos.y;
            if (socket.send(send_packet) != sf::Socket::Done){}
            send_packet.clear();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            player.move(my_client_id, -10.0f, 0.0f);
            packet_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << my_client_id << pos.x << pos.y;
            if (socket.send(send_packet) != sf::Socket::Done){}
            send_packet.clear();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            player.move(my_client_id, 0.0f, 10.0f);
            packet_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << my_client_id << pos.x << pos.y;
            if (socket.send(send_packet) != sf::Socket::Done){}
            send_packet.clear();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            player.move(my_client_id, 10.0f, 0.0f);
            packet_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << my_client_id << pos.x << pos.y;
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
                receive_packet >> client_id >> x >> y;
                receive_packet.clear();
                player.setPosition(client_id, x, y);
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

        window->clear(sf::Color::White);
        window->draw(sprite);
        window->draw(sprite2);
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