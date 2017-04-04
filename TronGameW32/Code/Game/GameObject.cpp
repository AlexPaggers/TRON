#include "GameObject.h"



GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

void GameObject::draw(sf::RenderWindow & _window)
{
	_window.draw(sprite);
}

void GameObject::tick(sf::RenderWindow & _window)
{
	//sprite.setPosition(m_pos);
	draw(_window);
}

