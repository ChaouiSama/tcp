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

    enum e_packets_types : sf::Uint8
    {
        PT_CLIENT_CONNECTION,
        PT_DATA_TRANSFERT,
        PT_CLIENT_DISCONNECTION,
        PT_SHIPS_PLACEMENT,
        PT_HIT_PLACEMENT,
        PT_NEW_TURN
    };

    sf::Uint8 packet_type;
    sf::Uint8 action_type;

    Player player;

    float x;
    float y;

    sf::Vector2f pos;

    bool moving(false);

    struct s_ships_and_hits
    {
        int mouseX;
        int mouseY;
        int x;
        int y;

        int max_ship_parts = 17;
        int used_ship_parts = 0;
        int ship_parts_placed_in_a_row = 0;
        int prevX, prevY;
        int dir = 0;
        bool is_ship_incorrect = false;
        bool is_placement_over = false;

        std::map<int, int> ships_available;

        std::vector<std::vector<int>> *board;
    };

    s_ships_and_hits ships_and_hits;

    for (int i(2); i <= 5; ++i)
    {
        if (i == 3)
            ships_and_hits.ships_available.emplace(i, 2);
        else
            ships_and_hits.ships_available.emplace(i, 1);
    }

    enum e_game_states : sf::Uint8
    {
        GS_SHIPS_PLACEMENT,
        GS_PLAYER1_TURN,
        GS_PLAYER2_TURN
    };

    e_game_states game_state;

    ships_and_hits.board = new std::vector<std::vector<int>>(10, std::vector<int>(10));

    for (int i(0); i < ships_and_hits.board->size(); ++i)
    {
        for (int j(0); j < ships_and_hits.board->at(i).size(); ++j)
        {
            ships_and_hits.board->at(i).at(j) = 0;
        }
    }

    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT, sf::VideoMode::getDesktopMode().bitsPerPixel), "test", sf::Style::Close, sf::ContextSettings(0, 0, 8, 1, 1, false));
    window->setFramerateLimit(120);

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

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Button::Left)
                {
                    if (ships_and_hits.ships_available.find(ships_and_hits.ship_parts_placed_in_a_row)->second <= 0 || ships_and_hits.is_ship_incorrect)
                    {
                        graphics.removeShip(ships_and_hits.ship_parts_placed_in_a_row);
                        ships_and_hits.max_ship_parts += ships_and_hits.ship_parts_placed_in_a_row;
                    }

                    if (!ships_and_hits.is_ship_incorrect)
                    {
                        ships_and_hits.ships_available.find(ships_and_hits.ship_parts_placed_in_a_row)->second--;
                        
                        switch (ships_and_hits.dir)
                        {
                        case 1:
                            for (int i(ships_and_hits.prevY - 10 - ships_and_hits.ship_parts_placed_in_a_row); i < ships_and_hits.prevY - 10; ++i)
                            {
                                ships_and_hits.board->at(i).at(ships_and_hits.prevX - 11) = 1;
                            }
                            break;

                        case 2:
                            for (int i(ships_and_hits.prevX - 10 - ships_and_hits.ship_parts_placed_in_a_row); i < ships_and_hits.prevX - 10; ++i)
                            {
                                ships_and_hits.board->at(ships_and_hits.prevY - 11).at(i) = 1;
                            }
                            break;

                        default:
                            break;
                        }
                    }

                    if (game_state == GS_SHIPS_PLACEMENT && ships_and_hits.used_ship_parts == ships_and_hits.max_ship_parts && !ships_and_hits.is_placement_over)
                    {
                        ships_and_hits.is_placement_over = true;
                        packet_type = PT_DATA_TRANSFERT;
                        action_type = PT_SHIPS_PLACEMENT;

                        send_packet << packet_type << action_type << my_client_id;

                        for (int i(0); i < ships_and_hits.board->size(); ++i)
                        {
                            for (int j(0); j < ships_and_hits.board->at(i).size(); ++j)
                            {
                                send_packet << ships_and_hits.board->at(i).at(j);
                                std::cout << ships_and_hits.board->at(i).at(j) << ' ';
                            }
                            std::cout << std::endl;
                        }

                        //send_packet << ships_and_hits.board;
                        client.sendData(send_packet);
                    }

                    ships_and_hits.ship_parts_placed_in_a_row = 0;
                    ships_and_hits.dir = 0;
                    ships_and_hits.is_ship_incorrect = false;
                }
                break;

            default:
                break;
            }
        }

        if (client.receiveData(&receive_packet))
        {
            receive_packet >> packet_type;

            switch ((e_packets_types)packet_type)
            {
            case PT_CLIENT_CONNECTION:
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

            case PT_DATA_TRANSFERT:
                receive_packet >> action_type;
                switch ((e_packets_types)action_type)
                {
                case PT_HIT_PLACEMENT:
                    receive_packet >> x >> y;
                    receive_packet.clear();
                    graphics.makeHitSprite((int)x + 10, (int)y + 10);
                    break;

                case PT_NEW_TURN:
                    receive_packet >> client_id;
                    receive_packet.clear();
                    if (client_id == my_client_id)
                    {
                        game_state = GS_PLAYER1_TURN;
                        std::cout << "MY TURN !" << std::endl;
                    }
                    else
                    {
                        game_state = GS_PLAYER2_TURN;
                        std::cout << "ENNEMY'S TURN !" << std::endl;
                    }
                    break;

                default:
                    break;
                }
                break;

            case PT_CLIENT_DISCONNECTION:
                receive_packet >> client_id;
                receive_packet.clear();
                std::cout << "deconnection packet received from client_id[" << client_id << "]" << std::endl;
                //player.getPlayerList()->erase(player.getPlayerList()->find(client_id));
                player.getPlayerList()->erase(std::find(player.getPlayerList()->begin(), player.getPlayerList()->end(), client_id));
                break;

            default:
                break;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && window->hasFocus() && game_state == GS_PLAYER1_TURN)
        {
            ships_and_hits.mouseX = sf::Mouse::getPosition(*window).x;
            ships_and_hits.mouseY = sf::Mouse::getPosition(*window).y;

            ships_and_hits.x = (ships_and_hits.mouseX / 38) + 1;
            ships_and_hits.y = (ships_and_hits.mouseY / 38) + 1;

            if (ships_and_hits.x > 0 && ships_and_hits.y > 0 && ships_and_hits.x <= 10 && ships_and_hits.y <= 10)
            {
                graphics.makeHitSprite(ships_and_hits.x, ships_and_hits.y);

                packet_type = PT_DATA_TRANSFERT;
                action_type = PT_HIT_PLACEMENT;
                send_packet << packet_type << action_type << my_client_id << (float)ships_and_hits.x << (float)ships_and_hits.y;
                client.sendData(send_packet);

                game_state = GS_PLAYER2_TURN;
            }
        }
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && window->hasFocus() && game_state == GS_SHIPS_PLACEMENT)
        {
            ships_and_hits.mouseX = sf::Mouse::getPosition(*window).x;
            ships_and_hits.mouseY = sf::Mouse::getPosition(*window).y;

            ships_and_hits.x = (ships_and_hits.mouseX / 38) + 1;
            ships_and_hits.y = (ships_and_hits.mouseY / 38) + 1;

            if (ships_and_hits.x > 10 && ships_and_hits.y > 10 && ships_and_hits.x <= 20 && ships_and_hits.y <= 20 && ships_and_hits.used_ship_parts < ships_and_hits.max_ship_parts)
            {
                for (std::map<int, int>::reverse_iterator iter(ships_and_hits.ships_available.rbegin()); iter != ships_and_hits.ships_available.rend(); ++iter)
                {
                    if (iter->second > 0)
                    {
                        if ((ships_and_hits.x != ships_and_hits.prevX || ships_and_hits.y != ships_and_hits.prevY) && ships_and_hits.ship_parts_placed_in_a_row < iter->first)
                        {
                            if (ships_and_hits.ship_parts_placed_in_a_row == 1)
                            {
                                if (ships_and_hits.x == ships_and_hits.prevX && ships_and_hits.y != ships_and_hits.prevY)
                                    ships_and_hits.dir = 1;
                                else if (ships_and_hits.x != ships_and_hits.prevX && ships_and_hits.y == ships_and_hits.prevY)
                                    ships_and_hits.dir = 2;
                            }

                            if (ships_and_hits.ship_parts_placed_in_a_row >= 1)
                            {
                                switch (ships_and_hits.dir)
                                {
                                case 1:
                                    if (ships_and_hits.x != ships_and_hits.prevX && ships_and_hits.y == ships_and_hits.prevY)
                                    {
                                        ships_and_hits.is_ship_incorrect = true;
                                    }
                                    break;

                                case 2:
                                    if (ships_and_hits.x == ships_and_hits.prevX && ships_and_hits.y != ships_and_hits.prevY)
                                    {
                                        ships_and_hits.is_ship_incorrect = true;
                                    }
                                    break;

                                default:
                                    break;
                                }
                            }

                            graphics.makeShipSprite(ships_and_hits.x, ships_and_hits.y);
                            ships_and_hits.prevX = ships_and_hits.x;
                            ships_and_hits.prevY = ships_and_hits.y;
                            ships_and_hits.used_ship_parts++;
                            ships_and_hits.ship_parts_placed_in_a_row++;
                        }

                        break;
                    }
                }
            }
        }

        window->clear(sf::Color::White);
        graphics.draw(window);
        window->display();
    }

    delete ships_and_hits.board;
    delete window;

    return 0;
}
