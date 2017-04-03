#include "Player.h"



void Player::tick(sf::RenderWindow &_window)
{
	
	switch (m_current_direction)
	{
		case CurrentDirection::DOWN:
			{
				sprite.move(sf::Vector2f(0.00f, 0.04f));
			}
		break;
		case CurrentDirection::LEFT:
			{
				sprite.move(sf::Vector2f(-0.04f, 0.00f));
			}
		break;
		case CurrentDirection::RIGHT:
			{
				sprite.move(sf::Vector2f(0.04f, 0.00f));
			}
		break;
		case CurrentDirection::UP:
			{
				sprite.move(sf::Vector2f(0.00f, -0.04f));
			}
		break;
		default:
			{

			}
		break;
	}

	draw(_window);
}

void Player::draw(sf::RenderWindow &_window)
{
	_window.draw(sprite);
}

Player::Player(PlayerColour _starting_colour)
{
	

	sprite.setPosition(sf::Vector2f(50, 50));
	//sprite.setScale(1, 1);

	if (!m_texture.loadFromFile("..\\..\\Resources\\player.png"))
	{
		return;
	}
	//m_rect.setTexture(&m_texture);
	sprite.setTexture(m_texture);
}

Player::~Player()
{

}
