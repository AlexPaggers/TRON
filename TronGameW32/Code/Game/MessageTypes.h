#pragma once
#include <SFML/Network.hpp>
#include <Game\MessageTypes.h>
enum NetMsg : sf::Int32
{
	INVALID			= 0,
	CHAT			= 1,
	PING			= 2,
	PONG			= 3,
	CLIENT_COUNT	= 4,
	UP				= 5,
	DOWN			= 6,
	LEFT			= 7,
	RIGHT			= 8,


};