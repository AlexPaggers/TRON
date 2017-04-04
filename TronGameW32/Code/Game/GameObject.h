#pragma once
#include <SFML/Graphics.hpp>

class GameObject : public sf::Sprite
{
public:
	
	enum PlayerColour
	{
		BLUE,
		ORANGE,
	};

	GameObject();
	virtual ~GameObject();

	virtual void draw(sf::RenderWindow &_window);	//do some draw
	virtual void tick(sf::RenderWindow &_window);	//do some logic

	sf::Vector2f getPos()			{ return m_pos; }
	void setPos(sf::Vector2f _pos)	{ m_pos = _pos; }

protected:
	sf::Sprite			sprite;
	sf::Vector2f		m_pos;
	sf::Texture			m_texture;
	sf::RectangleShape	m_rect;
};

