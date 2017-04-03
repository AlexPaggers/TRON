#pragma once
#include "GameObject.h"
#include "Player.h"


class Tile :
	public GameObject
{
public:
	Tile(int _index);
	Tile();
	~Tile();

	void setTexture(Player::PlayerColour _colour);

	bool isUsed() { return m_used; }

	void draw(sf::RenderWindow & _window) override;

private:

	const int m_width = 10;
	int m_index;

	bool m_used = false;

	sf::Texture m_blue;
	sf::Texture m_orange;
};

