#include <future>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

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

Grid * p_grid = new Grid();
Player * p_player	= new Player(GameObject::PlayerColour::BLUE);



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
				packet >> header;

				NetMsg msg = static_cast<NetMsg>(header);
				if (msg == NetMsg::CHAT)
				{
					std::string str;
					packet >> str;
					std::cout << "< " << str << std::endl;
				}
				else if (msg == NetMsg::PING)
				{
					sf::Packet pong;
					pong << NetMsg::PONG;
					socket.send(pong);
				}
				else if (msg == NetMsg::RIGHT)
				{

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
			p_player->getCurrentDirection() != Player::CurrentDirection::UP &&
			p_player->getGridPos() > p_grid->m_tiles[p_player->getGridPos()].getWidth())
		{
			packet << NetMsg::UP << input;
			p_player->setDirection(Player::CurrentDirection::UP);
			p_grid->m_tiles[p_player->getGridPos()].setTexture(p_player->getPlayerColour());
			p_player->setGridPos((p_player->getGridPos() - p_grid->m_tiles[p_player->getGridPos()].getWidth()), p_grid->m_tiles);
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
				p_player->getCurrentDirection() != Player::CurrentDirection::LEFT)
		{
			packet << NetMsg::LEFT << input;
			p_player->setDirection(Player::CurrentDirection::LEFT);
			p_grid->m_tiles[p_player->getGridPos()].setTexture(p_player->getPlayerColour());
			p_player->setGridPos((p_player->getGridPos() - 1), p_grid->m_tiles);
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
				p_player->getCurrentDirection() != Player::CurrentDirection::DOWN)
		{
			packet << NetMsg::DOWN << input;
			p_player->setDirection(Player::CurrentDirection::DOWN);
			p_grid->m_tiles[p_player->getGridPos()].setTexture(p_player->getPlayerColour());
			p_player->setGridPos((p_player->getGridPos() + p_grid->m_tiles[p_player->getGridPos()].getWidth()), p_grid->m_tiles);
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) &&
				p_player->getCurrentDirection() != Player::CurrentDirection::RIGHT)
		{
			
			packet << NetMsg::RIGHT << input;
			p_player->setDirection(Player::CurrentDirection::RIGHT);
			p_grid->m_tiles[p_player->getGridPos()].setTexture(p_player->getPlayerColour());
			p_player->setGridPos((p_player->getGridPos() + 1), p_grid->m_tiles);
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
			packet << NetMsg::PING << input;
			socket.send(packet);
			clock.restart();
		}

		window.clear();
		p_player->tick(window);
		for (int i = 0; i < p_grid->m_tiles.size(); i++)
		{
			//p_grid->m_tiles[i].setTexture(Player::PlayerColour::BLUE);
			p_grid->m_tiles[i].tick(window);
		}
		window.display();

	}
}