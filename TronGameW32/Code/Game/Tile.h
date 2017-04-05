#pragma once
#include "GameObject.h"

class Tile :
	public GameObject
{
public:
	Tile(int _index);
	Tile();
	~Tile();

	int getWidth()		{ return m_width; }

	void setTexture(GameObject::PlayerColour _colour);

	void makeUsed()		{ m_used = true; }
	bool isUsed()		{ return m_used; }

	void draw(sf::RenderWindow & _window) override;

private:

	const int m_width = 14;
	int m_index;

	bool m_used = false;

	sf::Texture m_blue;
	sf::Texture m_orange;
	sf::Texture m_wall;
};

