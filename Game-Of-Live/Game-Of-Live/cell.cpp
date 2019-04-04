#include "pch.h"
#include "cell.h"


bool cell::get_state()
{
	return is_alive;
}

void cell::set_state(bool state)
{
	if(is_alive != state)
		is_alive = state;

	if(state)
		shape.setFillColor(sf::Color(0, 128, 255));
	else
		shape.setFillColor(sf::Color(250, 250, 250));
}

void cell::set_position(int x, int y)
{
	shape.setPosition(20 + x, 70+y);
}

void cell::set_click()
{
	if (is_alive)
		set_state(false);
	else
		set_state(true);
}

void cell::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(shape);
}

bool cell::getBounds(sf::Vector2f &m)
{
	return shape.getGlobalBounds().contains(m);
}

cell::cell()
{
	is_alive = false;
	shape.setSize(sf::Vector2f(10, 10));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color::Black);
	shape.setFillColor(sf::Color(250, 250, 250));
}


cell::~cell()
{
}
