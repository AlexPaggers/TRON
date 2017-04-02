#pragma once
#include "GameObject.h"


class Player : public GameObject
{
public:
	
	enum CurrentDirection
	{
		UP,
		RIGHT,
		DOWN,
		LEFT,
		NONE,
	};

	void tick(sf::RenderWindow &_window) override;
	void draw(sf::RenderWindow &_window) override;

	CurrentDirection	getCurrentDirection()								{ return m_current_direction; }
	void				setDirection(CurrentDirection _current_direction)	{ m_current_direction = _current_direction; }

	Player();
	~Player();

private:

	CurrentDirection m_current_direction = CurrentDirection::NONE;
};

