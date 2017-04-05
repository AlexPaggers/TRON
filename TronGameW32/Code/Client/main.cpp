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
				int header = 0;
				int clientID = 0;

				packet >> header;
				packet >> clientID;

				if (clientID > 1)
				{
					clientID = 1;
				}

				NetMsg msg = static_cast<NetMsg>(header);
				
				 if (msg == NetMsg::UP)
				{
					//packet >> clientID;

					p_grid->m_tiles[p_players[clientID].getGridPos()].setTexture(p_players[clientID].getPlayerColour());
					p_players[clientID].setGridPos((p_players[clientID].getGridPos() - p_grid->m_tiles[p_players[clientID].getGridPos()].getWidth()), p_grid->m_tiles);
				}
				else if (msg == NetMsg::RIGHT)
				{
					//packet >> clientID;

					p_grid->m_tiles[p_players[clientID].getGridPos()].setTexture(p_players[clientID].getPlayerColour());
					p_players[clientID].setGridPos((p_players[clientID].getGridPos() + 1), p_grid->m_tiles);
				}
			else if (msg == NetMsg::DOWN)
				{
					//packet >> clientID;

					p_grid->m_tiles[p_players[clientID].getGridPos()].setTexture(p_players[clientID].getPlayerColour());
					p_players[clientID].setGridPos((p_players[clientID].getGridPos() + p_grid->m_tiles[p_players[clientID].getGridPos()].getWidth()), p_grid->m_tiles);
				}
				else if (msg == NetMsg::LEFT)
				{
					//packet >> clientID;

					p_grid->m_tiles[p_players[clientID].getGridPos()].setTexture(p_players[clientID].getPlayerColour());
					p_players[clientID].setGridPos((p_players[clientID].getGridPos() - 1), p_grid->m_tiles);
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
			packet << NetMsg::UP << input;
			
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			packet << NetMsg::LEFT << input;

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			packet << NetMsg::DOWN << input;

		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			
			packet << NetMsg::RIGHT << input;

		}
		
		else
		{
			packet << NetMsg::INVALID << input;
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

	p_players[0].setGridPos(0, p_grid->m_tiles);
	p_players[1].setGridPos(16, p_grid->m_tiles);
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
		if (elapsed1.asSeconds() > 1)
		{
			sf::Packet packet;
			packet << NetMsg::PING;
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
		window.display();

	}

	delete p_grid;
	p_grid = nullptr;



}

