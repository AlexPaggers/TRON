#include <future>
#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Game/MessageTypes.h>


using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<TcpClientPtr>;

const sf::IpAddress SERVER_IP("127.0.0.1");
constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);

void client();
bool connect(TcpClient&);
void input(TcpClient&);

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
			}
		} while (status != sf::Socket::Disconnected);

	});

	return input(socket);
}

void input(TcpClient &socket)
{
	while (true)
	{

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			sf::Packet packet;
			packet << NetMsg::UP << input;
			socket.send(packet);

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			sf::Packet packet;
			packet << NetMsg::LEFT << input;
			socket.send(packet);

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			sf::Packet packet;
			packet << NetMsg::DOWN << input;
			socket.send(packet);

		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			sf::Packet packet;
			packet << NetMsg::RIGHT << input;
			socket.send(packet);

		}

		//sf::Packet packet;
		//packet << NetMsg::CHAT << input;
		//socket.send(packet);
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(700, 700), "ISTHISAJOJOREFERENCE?");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.display();

		client();

	}

	return 0;
}

