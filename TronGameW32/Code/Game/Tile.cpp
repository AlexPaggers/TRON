#include "Tile.h"



Tile::Tile(int _index)
{
	if (!m_blue.loadFromFile("..\\..\\Resources\\blue.png"))
	{
		return;
	}

	if (!m_orange.loadFromFile("..\\..\\Resources\\orange.png"))
	{
		return;
	}

	m_index = _index;

	m_pos.x = (m_index % m_width) * 51;
	m_pos.y = (m_index / m_width) * 51;


	sprite.setPosition(m_pos);

}



Tile::Tile()
{
	
}



Tile::~Tile()
{

}



void Tile::setTexture(Player::PlayerColour _colour)
{
	switch (_colour)
	{
		case Player::PlayerColour::BLUE:
		{
			sprite.setTexture(m_blue);
		}
		break;
		case Player::PlayerColour::ORANGE:
		{
			sprite.setTexture(m_orange);
		}
		break;
		default:
		break;
	}

	m_used = true;

}

void Tile::draw(sf::RenderWindow & _window)
{
	_window.draw(sprite);
}