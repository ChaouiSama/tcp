//#include "ClientNetworkManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    const int WIDTH(842), HEIGHT(842);
    
    sf::IpAddress address("192.168.1.34");
    unsigned short port(3000);

    int my_client_id, client_id;

    bool first_connection(true);

    sf::Packet send_packet, receive_packet;

    sf::Uint8 packet_type;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Sprite sprite2;

    //ClientNetworkManager client(address, port);

    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, sf::VideoMode::getDesktopMode().bitsPerPixel), "test", sf::Style::Close, sf::ContextSettings(0, 0, 8, 1, 1, false));
    window->setFramerateLimit(60);
    
    //client.connect();

    sf::TcpSocket socket;
    socket.setBlocking(false);

    if (socket.connect(address, port) == sf::Socket::Done)
    {
        std::cout << "connected" << std::endl;
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
                //client.disconnect(my_client_id);
                window->close();
                break;
                
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    //client.disconnect(my_client_id);
                    packet_type = 2;
                    send_packet << packet_type << my_client_id;
                    if (socket.send(send_packet) == sf::Socket::Done)
                    {
                        send_packet.clear();
                        window->close();
                    }
                }
                break;

            default:
                break;
            }
        }

        //receive_packet = client.receiveData();
        //receive_packet >> packet_type;

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
                }
                else
                {
                    receive_packet >> client_id;
                    receive_packet.clear();
                    std::cout << "client_id: " << client_id << std::endl;
                }
                break;

            case 1:
                break;

            case 2:
                receive_packet >> client_id;
                receive_packet.clear();
                std::cout << "deconnection packet received from client_id[" << client_id << "]" << std::endl;
                break;

            default:
                break;
            }
        }

        texture.loadFromFile("grid.png");
        sprite.setTexture(texture);
        sprite.setPosition(0, 0);
        sprite2.setTexture(texture);
        sprite2.setPosition(420, 420);

        window->clear(sf::Color::White);
        window->draw(sprite);
        window->draw(sprite2);
        window->display();
    }

    delete window;

    return 0;
}