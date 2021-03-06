// ChatServer.cpp : Defines the entry point for the console application.

#include "stdafx.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <SFML\Network.hpp>
#include <SFML\Window.hpp>

#include <Game\MessageTypes.h>
#include "Client.h"

constexpr int SERVER_TCP_PORT(53000);
constexpr int SERVER_UDP_PORT(53001);

using TcpClient = sf::TcpSocket;
using TcpClientPtr = std::unique_ptr<TcpClient>;
using TcpClients = std::vector<Client>;

// prototypes
bool bindServerPort(sf::TcpListener&);
void clearStaleCli(TcpClients & tcp_clients);
void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients);
void listen(sf::TcpListener&, sf::SocketSelector&, TcpClients&);
void processChatMsg(sf::Packet &packet, Client & sender, TcpClients & tcp_clients);
void ping(TcpClients& tcp_clients);
void receiveMsg(TcpClients& tcp_clients, sf::SocketSelector& selector);
void runServer();


sf::Int8 header = 0;


void ping(TcpClients& tcp_clients)
{
	constexpr auto timeout = 10s;
	for (auto& client : tcp_clients)
	{
		const auto& timestamp = client.getPingTime();
		const auto  now = std::chrono::steady_clock::now();
		auto delta = now - timestamp;
		if (delta > timeout)
		{
			client.ping();
		}
	}
}

void runServer()
{
	sf::TcpListener tcp_listener;

	if (!bindServerPort(tcp_listener))
	{
		return;
	}

	sf::SocketSelector selector;
	selector.add(tcp_listener);

	TcpClients tcp_clients;
	return listen(tcp_listener, selector, tcp_clients);
}

void listen(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients)
{
	while (true)
	{

		const sf::Time timeout = sf::Time(sf::milliseconds(250));
		if (selector.wait(timeout))
		{
			if (selector.isReady(tcp_listener))
			{
				connect(tcp_listener, selector, tcp_clients);
			}
			else
			{
				receiveMsg(tcp_clients, selector);
				clearStaleCli(tcp_clients);
			}
		}
		else
		{
			ping(tcp_clients);
		}

	}
}

void connect(sf::TcpListener& tcp_listener, sf::SocketSelector& selector, TcpClients& tcp_clients)
{
	auto  client_ptr = new sf::TcpSocket;
	auto& client_ref = *client_ptr;
	if (tcp_listener.accept(client_ref) == sf::Socket::Done)
	{
		selector.add(client_ref);

		auto client = Client(client_ptr);
		tcp_clients.push_back(std::move(client));
		std::cout << "client connected" << std::endl;

		std::string welcome_msg;
		std::string client_count = std::to_string(tcp_clients.size());

		sf::Packet packet;
		packet << NetMsg::CHAT << welcome_msg;
		client_ref.send(packet);
	}
}

void receiveMsg(TcpClients& tcp_clients, sf::SocketSelector& selector)
{
	for (auto& sender : tcp_clients)
	{
		auto& sender_socket = sender.getSocket();
		if (selector.isReady(sender_socket))
		{
			sf::Packet packet;
			auto status = sender_socket.receive(packet);
			if (status == sf::Socket::Disconnected)
			{
				selector.remove(sender_socket);
				sender_socket.disconnect();
				std::cout << "Client (" << sender.getClientID()
					<< ") Disconnected" << std::endl;
				break;
			}
			if (status == sf::Socket::Done)
			{
				packet >> header;      ////////

				NetMsg msg = static_cast<NetMsg>(header);
				if (msg == NetMsg::CHAT)
				{
					processChatMsg(packet, sender, tcp_clients);
				}
				else if (msg == NetMsg::PING)
				{
					processChatMsg(packet, sender, tcp_clients);
				}
				else if (msg == NetMsg::UP)
				{
					std::cout << "Client (" << sender.getClientID() << ") -" << "UP" << std::endl;
					sender.setCurrentDirection(NetMsg::UP);
				}
				else if (msg == NetMsg::LEFT)
				{
					std::cout << "Client (" << sender.getClientID() << ") -" << "LEFT" << std::endl;
					sender.setCurrentDirection(NetMsg::LEFT);
				}
				else if (msg == NetMsg::DOWN)
				{
					std::cout << "Client (" << sender.getClientID() << ") -" << "DOWN" << std::endl;
					sender.setCurrentDirection(NetMsg::DOWN);
				}
				else if (msg == NetMsg::RIGHT)
				{
					std::cout << "Client (" << sender.getClientID() << ") -" << "RIGHT" << std::endl;
					sender.setCurrentDirection(NetMsg::RIGHT);
				}
			}


		}
	}
}

void clearStaleCli(TcpClients & tcp_clients)
{
	tcp_clients.erase(
		std::remove_if(tcp_clients.begin(), tcp_clients.end(), [](const Client& client)
	{
		return(!client.isConnected());
	}), tcp_clients.end());
}

void processChatMsg(sf::Packet &packet, Client & sender, TcpClients & tcp_clients)
{
	sf::Packet client_info;

	NetMsg msg = static_cast<NetMsg>(header);
	if (header == NetMsg::PING && 
		sender.getClientID() == 0 &&
		tcp_clients.size() > 1)
	{
		std::cout << "Step" << std::endl;
		for (auto& client : tcp_clients)
		{
			sf::Int8 id;
			sf::Int8 dir;
			id = client.getClientID();
			dir = static_cast<sf::Int8>(client.getCurrentDirection());
			client_info << dir;
			client_info << id;
		}
		for (auto& client : tcp_clients)
		{
			sf::Int8 id;
			id = client.getClientID();
			client.getSocket().send(client_info);
		}
	}



	// send the packet to other clients


	header = 0;

}

bool bindServerPort(sf::TcpListener& listener)
{
	if (listener.listen(SERVER_TCP_PORT) != sf::Socket::Done)
	{
		std::cout << "Could not bind server port, is another app using it?";
		std::cout << std::endl << "Port: " << SERVER_TCP_PORT;
		std::cout << std::endl;
		return false;
	}

	std::cout << "Server launched on port: " << SERVER_TCP_PORT << std::endl;;
	std::cout << "Searching for extraterrestrial life..." << std::endl;
	return true;
}

int main()
{
	runServer();
	return 0;
}
