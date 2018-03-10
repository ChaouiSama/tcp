#include "ClientNetworkManager.hpp"
#include "Player.hpp"
#include "Graphics.hpp"
#include <cmath>

int main(int argc, char* argv[])
{
    const int WIDTH(762), HEIGHT(762);

    sf::IpAddress address("chaouisama.tk");
    unsigned short port(2000);

    int my_client_id, client_id;

    bool first_connection(true);

    sf::Packet send_packet, receive_packet;

    sf::Uint8 packet_type;
    sf::Uint8 action_type;

    Player player;

    float x;
    float y;

    sf::Vector2f pos;

    bool moving(false);

    int max_ship_parts(17);
    int used_ship_parts(0);
    int ship_parts_placed_in_a_row(0);
    int prevX, prevY;
    int dir(0);
    bool is_ship_incorrect(false);

    std::map<int, int> ships_available;

    for (int i(2); i <= 5; ++i)
    {
        if (i == 3)
            ships_available.emplace(i, 2);
        else
            ships_available.emplace(i, 1);
    }

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, sf::VideoMode::getDesktopMode().bitsPerPixel), "test", sf::Style::Close, sf::ContextSettings(0, 0, 8, 1, 1, false));
    window->setFramerateLimit(500);

    Graphics graphics;

    graphics.loadTexture("data/grid.png");

    for (int i(0); i <= 1; ++i)
    {
        graphics.makeGridSprite(i);
    }

    ClientNetworkManager client;
    client.connect(address, port);

    while (window->isOpen())
    {
        sf::Event event;

        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                client.disconnect();
                window->close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    client.disconnect();
                    window->close();
                }
                break;

            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::W && window->hasFocus())
                {
                    moving = false;
                }
                if (event.key.code == sf::Keyboard::Q && window->hasFocus())
                {
                    moving = false;
                }
                if (event.key.code == sf::Keyboard::S && window->hasFocus())
                {
                    moving = false;
                }
                if (event.key.code == sf::Keyboard::D && window->hasFocus())
                {
                    moving = false;
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                    if (ships_available.find(ship_parts_placed_in_a_row)->second <= 0 || is_ship_incorrect)
                    {
                        graphics.removeShip(ship_parts_placed_in_a_row);
                        max_ship_parts += ship_parts_placed_in_a_row;
                    }

                    if (!is_ship_incorrect)
                    {
                        ships_available.find(ship_parts_placed_in_a_row)->second--;
                    }

                    ship_parts_placed_in_a_row = 0;
                    dir = 0;
                    is_ship_incorrect = false;
                }
                break;

            default:
                break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && window->hasFocus())
        {
            moving = true;
            player.move(my_client_id, 0.0f, -10.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && window->hasFocus())
        {
            player.move(my_client_id, -10.0f, 0.0f);
            moving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && window->hasFocus())
        {
            player.move(my_client_id, 0.0f, 10.0f);
            moving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && window->hasFocus())
        {
            player.move(my_client_id, 10.0f, 0.0f);
            moving = true;
        }

        if (moving)
        {
            packet_type = 0;
            action_type = 0;
            pos = player.getPosition(my_client_id);
            send_packet << packet_type << action_type << my_client_id << pos.x << pos.y;
            client.sendData(send_packet);
        }

        if (client.receiveData(&receive_packet))
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
            int mouseX = sf::Mouse::getPosition(*window).x;
            int mouseY = sf::Mouse::getPosition(*window).y;

            int x = (mouseX / 38) + 1;
            int y = (mouseY / 38) + 1;
            if (x > 0 && y > 0 && x <= 10 && y <= 10)
            {
                graphics.makeHitSprite(x, y);

                packet_type = 0;
                action_type = 1;
                send_packet << packet_type << action_type << my_client_id << (float)x << (float)y;
                client.sendData(send_packet);
            }
            else if (x > 10 && y > 10 && x <= 20 && y <= 20 && used_ship_parts < max_ship_parts)
            {
                for (std::map<int, int>::reverse_iterator iter(ships_available.rbegin()); iter != ships_available.rend(); ++iter)
                {
                    if (iter->second > 0)
                    {
                        if ((x != prevX || y != prevY) && ship_parts_placed_in_a_row < iter->first)
                        {
                            if (ship_parts_placed_in_a_row == 1)
                            {
                                if (x == prevX && y != prevY)
                                    dir = 1;
                                else if (x != prevX && y == prevY)
                                    dir = 2;
                            }

                            if (ship_parts_placed_in_a_row >= 1)
                            {
                                switch (dir)
                                {
                                case 1:
                                    if (x != prevX && y == prevY)
                                    {
                                        is_ship_incorrect = true;
                                    }
                                    break;

                                case 2:
                                    if (x == prevX && y != prevY)
                                    {
                                        is_ship_incorrect = true;
                                    }
                                    break;

                                default:
                                    break;
                                }
                            }

                            graphics.makeShipSprite(x, y);
                            prevX = x;
                            prevY = y;
                            used_ship_parts++;
                            ship_parts_placed_in_a_row++;
                        }

                        break;
                    }
                }
            }
        }


        window->clear(sf::Color::White);
        graphics.draw(window);
        for (std::map<int, sf::CircleShape>::iterator iter(player.getPlayerList()->begin()); iter != player.getPlayerList()->end(); ++iter)
        {
            window->draw(iter->second);
        }
        window->display();
    }

    delete window;

    return 0;
}
