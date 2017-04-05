#include <future>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window/Joystick.hpp>


#include <Game/MessageTypes.h>

#include <Game/GameObject.h>
#include <Game/Player.h>
#include <Game/Grid.h>


using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<TcpClientPtr>;

const sf::IpAddress SERVER_IP("127.0.0.1");
constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);

void client();
bool connect(TcpClient&);
void input(TcpClient&);
void rendering(TcpClient&);
void move();

Grid * p_grid = new Grid();
std::vector<Player> p_players;

sf::Time elapsed1;
bool p_game_over = false;
bool p_blue_win;

bool connect(TcpClient& socket)
{
	auto status = socket.connect(SERVER_IP, SERVER_TCP_PORT);
	if (status != sf::Socket::Done)
	{
		return false;
	}

	std::cout << "Connected to server: " << SERVER_IP << std::endl;
	socket.setBlocking(false);
	return true;
}

void client()
{
	TcpClient socket;
	if (!connect(socket))
	{
		return;
	}

	auto handle = std::async(std::launch::async, [&]
	{
		// keep track of the socket status
		sf::Socket::Status status;

		do
		{
			sf::Packet packet;
			status = socket.receive(packet);
			if (status == sf::Socket::Done)
			{

				for (int i = 0; i < 2; i++)
				{
					sf::Int8 header = 0;
					sf::Int8 clientID = 0;

					packet >> header;
					packet >> clientID;

					//if (clientID > 1)
					//{
					//	clientID = 1;
					//}

					NetMsg msg = static_cast<NetMsg>(header);

					if (msg == NetMsg::UP)
					{
						//packet >> clientID;
						if (clientID == 0)
						{
							p_grid->m_tiles[p_players[0].getGridPos()].setTexture(p_players[0].getPlayerColour());
							p_players[0].setGridPos((p_players[0].getGridPos() - p_grid->m_tiles[p_players[0].getGridPos()].getWidth()), p_grid->m_tiles);
							
							if (p_grid->m_tiles[p_players[0].getGridPos()].isUsed())
							{
								//BLUE WIN
								p_game_over = true;
								p_blue_win;
							}

							p_grid->m_tiles[p_players[0].getGridPos()].makeUsed();

						}
						else
						{
							p_grid->m_tiles[p_players[1].getGridPos()].setTexture(p_players[1].getPlayerColour());
							p_players[1].setGridPos((p_players[1].getGridPos() - p_grid->m_tiles[p_players[1].getGridPos()].getWidth()), p_grid->m_tiles);

							if (p_grid->m_tiles[p_players[1].getGridPos()].isUsed())
							{
								//ORANGE WIN
								p_game_over = true;
							}

							p_grid->m_tiles[p_players[1].getGridPos()].makeUsed();
						}

					}
					if (msg == NetMsg::RIGHT)
					{
						//packet >> clientID;
						if (clientID == 0)
						{
							p_grid->m_tiles[p_players[0].getGridPos()].setTexture(p_players[0].getPlayerColour());
							p_players[0].setGridPos((p_players[0].getGridPos() + 1), p_grid->m_tiles);

							if (p_grid->m_tiles[p_players[0].getGridPos()].isUsed())
							{
								//BLUE WIN
								p_game_over = true;
								p_blue_win;
							}

							p_grid->m_tiles[p_players[0].getGridPos()].makeUsed();

						}
						else
						{
							p_grid->m_tiles[p_players[1].getGridPos()].setTexture(p_players[1].getPlayerColour());
							p_players[1].setGridPos((p_players[1].getGridPos() + 1), p_grid->m_tiles);

							if (p_grid->m_tiles[p_players[1].getGridPos()].isUsed())
							{
								//ORANGE WIN
								p_game_over = true;
							}

							p_grid->m_tiles[p_players[1].getGridPos()].makeUsed();

						}

					}
					if (msg == NetMsg::DOWN)
					{
						//packet >> clientID;
						if (clientID == 0)
						{
							p_grid->m_tiles[p_players[0].getGridPos()].setTexture(p_players[0].getPlayerColour());
							p_grid->m_tiles[p_players[0].getGridPos()].makeUsed();
							p_players[0].setGridPos((p_players[0].getGridPos() + p_grid->m_tiles[p_players[0].getGridPos()].getWidth()), p_grid->m_tiles);

							if (p_grid->m_tiles[p_players[0].getGridPos()].isUsed())
							{
								//BLUE WIN
								p_game_over = true;
								p_blue_win;
							}

							p_grid->m_tiles[p_players[0].getGridPos()].makeUsed();

						}
						else
						{
							p_grid->m_tiles[p_players[1].getGridPos()].setTexture(p_players[1].getPlayerColour());
							p_players[1].setGridPos((p_players[1].getGridPos() + p_grid->m_tiles[p_players[1].getGridPos()].getWidth()), p_grid->m_tiles);

							if (p_grid->m_tiles[p_players[1].getGridPos()].isUsed())
							{
								//ORANGE WIN
								p_game_over = true;
							}

							p_grid->m_tiles[p_players[1].getGridPos()].makeUsed();


						}

					}
					if (msg == NetMsg::LEFT)
					{
						//packet >> clientID;
						if (clientID == 0)
						{
							
							p_grid->m_tiles[p_players[0].getGridPos()].setTexture(p_players[0].getPlayerColour());
							p_players[0].setGridPos((p_players[0].getGridPos() - 1), p_grid->m_tiles);

							if (p_grid->m_tiles[p_players[0].getGridPos()].isUsed())
							{
								//BLUE WIN
								p_game_over = true;
								p_blue_win;
							}

							p_grid->m_tiles[p_players[0].getGridPos()].makeUsed();

						}
						else
						{
							p_grid->m_tiles[p_players[1].getGridPos()].setTexture(p_players[1].getPlayerColour());
							p_players[1].setGridPos((p_players[1].getGridPos() - 1), p_grid->m_tiles);

							if (p_grid->m_tiles[p_players[1].getGridPos()].isUsed())
							{
								//ORANGE WIN
								p_game_over = true;
							}

							p_grid->m_tiles[p_players[1].getGridPos()].makeUsed();


						}


					}
				}
			}
		} while (status != sf::Socket::Disconnected);

	});

	return rendering(socket);
}

void input(TcpClient &socket)
{
	//while (true)
	{
		sf::Packet packet;


		sf::Joystick::update();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			packet << static_cast<sf::Int8>(NetMsg::UP);
			//packet << input;
			
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			packet << static_cast<sf::Int8>(NetMsg::LEFT);

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			packet << static_cast<sf::Int8>(NetMsg::DOWN);
			//packet << input;

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			
			packet << static_cast<sf::Int8>(NetMsg::RIGHT);

		}
		
		else
		{
			packet << static_cast<sf::Int8>(NetMsg::INVALID);
		}

		socket.send(packet);

		//packet << NetMsg::CHAT << input;

	}
} 

int main()
{


	p_players.push_back(Player(GameObject::PlayerColour::BLUE));
	p_players.push_back(Player(GameObject::PlayerColour::ORANGE));

	p_players.reserve(10);

	p_players[0].setGridPos(15, p_grid->m_tiles);
	p_players[1].setGridPos(40, p_grid->m_tiles);
	client();
}

void rendering(TcpClient &socket)
{
	sf::RenderWindow window(sf::VideoMode(700, 700), "ISTHISAJOJOREFERENCE?");
	sf::Clock clock;
	while (window.isOpen())
	{

		
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{

				input(socket);

			}
		
		}

		elapsed1 = clock.getElapsedTime();
		if (elapsed1.asSeconds() > 1 &&
			!p_game_over)
		{
			sf::Packet packet;
			packet << static_cast<sf::Int8>(NetMsg::PING);
			socket.send(packet);
			clock.restart();
		}

		window.clear();


		for (auto &player : p_players)
		{
			player.tick(window);
		}
		for (int i = 0; i < p_grid->m_tiles.size(); i++)
		{
			//p_grid->m_tiles[i].setTexture(Player::PlayerColour::BLUE);
			p_grid->m_tiles[i].tick(window);
		}


		if (p_game_over)
		{
			sf::Text game_over_text;
			sf::Font game_over_font;
			game_over_font.loadFromFile("..\\..\\Resources\\crackman.ttf");
			
			game_over_text.setFont(game_over_font);
			game_over_text.setCharacterSize(40);
			game_over_text.setOutlineColor(sf::Color::Black);
			game_over_text.setPosition(0, 0);

			if (p_blue_win)
			{
				game_over_text.setString("BLUE WINS");
				game_over_text.setFillColor(sf::Color::Blue);
			}
			else
			{
				game_over_text.setString("ORANGE WINS");
				game_over_text.setFillColor(sf::Color::Red);
			}

			window.draw(game_over_text);



		}


		window.display();


	}

	delete p_grid;
	p_grid = nullptr;



}

