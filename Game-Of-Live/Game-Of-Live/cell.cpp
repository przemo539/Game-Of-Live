#include "pch.h"
#include "cell.h"

unsigned int cell::marg = 0;
unsigned int cell::size = 12;
bool cell::get_state()
{
	return is_alive;
}

void cell::set_changed(bool state)
{
	changed = state;
}


void cell::set_state(bool state)
{
	is_alive = state;
	changed = true;

	if(state)
		shape.setFillColor(sf::Color(0, 128, 255));
	else
		shape.setFillColor(sf::Color(250, 250, 250));
}

void cell::set_position(int x, int y)
{
	shape.setPosition((size+1)*x+marg, 70 + (size+1) *y);
}

void cell::set_click()
{
	if (is_alive)
		set_state(false);
	else
		set_state(true);
}

bool cell::isChanged()
{
	return changed;
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

void cell::setMarg(unsigned int m)
{
	marg = m;
}

void cell::setSize(unsigned int s)
{
	size = s;
}

cell::cell()
{
	changed = false;
	is_alive = false;
	shape.setSize(sf::Vector2f(size, size));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color::Black);
	shape.setFillColor(sf::Color(250, 250, 250));
}

cell::cell(unsigned int x, unsigned int y)
{
	changed = false;
	is_alive = false;
	shape.setSize(sf::Vector2f(size, size));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color::Black);
	shape.setFillColor(sf::Color(250, 250, 250));
	shape.setPosition((size+1)* x + marg, 70 + (size+1) * y);
}


cell::~cell()
{
}
