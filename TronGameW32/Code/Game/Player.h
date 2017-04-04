#pragma once
#include "GameObject.h"
#include <Game/Grid.h>

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

	// Get/Set Player Direction
	CurrentDirection	getCurrentDirection()								{ return m_current_direction; }
	void				setDirection(CurrentDirection _current_direction)	{ m_current_direction = _current_direction; }

	// Get/Set Player Colour
	PlayerColour		getPlayerColour()									{ return m_current_colour; }
	void				setColour(PlayerColour _current_colour)				{ m_current_colour = _current_colour; }

	// Get/Set Player's position on the grid
	int					getGridPos()										{ return m_current_coord; }
	void				setGridPos(int _index, std::vector<Tile> _tiles);

	Player	() = default;
	Player	(GameObject::PlayerColour _starting_colour);
	~Player	();

private:

	CurrentDirection	m_current_direction = CurrentDirection::NONE;
	PlayerColour		m_current_colour;

	int m_current_coord						= 0;
};

