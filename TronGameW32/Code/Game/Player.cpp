#include "Player.h"



void Player::tick(sf::RenderWindow &_window)
{
	//m_rect.setPosition(m_pos);
	sprite.move(sf::Vector2f(0.01, 0.01));
	draw(_window);
}

void Player::draw(sf::RenderWindow &_window)
{
	_window.draw(sprite);
}

Player::Player()
{
	m_pos = sf::Vector2f(0, 0);
	
	sprite.setPosition(sf::Vector2f(50, 50));
	sprite.setScale(0.1, 0.1);

	if (!m_texture.loadFromFile("..\\..\\Resources\\player.png"))
	{
		return;
	}
	m_rect.setTexture(&m_texture);
	sprite.setTexture(m_texture);
}

Player::~Player()
{

}
