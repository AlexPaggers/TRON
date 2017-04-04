#include "Player.h"



void Player::tick(sf::RenderWindow &_window)
{
	
	//switch (m_current_direction)
	//{
	//	case CurrentDirection::DOWN:
	//		{
	//			sprite.move(sf::Vector2f(0.00f, 0.04f));
	//		}
	//	break;
	//	case CurrentDirection::LEFT:
	//		{
	//			sprite.move(sf::Vector2f(-0.04f, 0.00f));
	//		}
	//	break;
	//	case CurrentDirection::RIGHT:
	//		{
	//			sprite.move(sf::Vector2f(0.04f, 0.00f));
	//		}
	//	break;
	//	case CurrentDirection::UP:
	//		{
	//			sprite.move(sf::Vector2f(0.00f, -0.04f));
	//		}
	//	break;
	//	default:
	//		{
	//			//
	//		}
	//	break;
	//}

	draw(_window);
}

void Player::draw(sf::RenderWindow &_window)
{
	_window.draw(sprite);
}

void Player::setGridPos(int _index, std::vector<Tile> _tiles) 
{
	m_current_coord = _index;
	sprite.setPosition(_tiles[_index].getPos());
}

Player::Player(GameObject::PlayerColour _starting_colour)
{	

	sprite.setPosition(sf::Vector2f(0, 0));

	m_current_colour = _starting_colour;

	if (!m_texture.loadFromFile("..\\..\\Resources\\player.png"))
	{
		return;
	}

	sprite.setTexture(m_texture);
}

Player::~Player()
{

}
