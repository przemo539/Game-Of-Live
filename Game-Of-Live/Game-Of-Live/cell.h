#include <SFML/Graphics.hpp>
#pragma once
class cell : public sf::Drawable, sf::Transformable
{
	bool is_alive;
public:
	sf::RectangleShape shape;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	bool get_state();
	void set_state(bool);
	void set_position(int, int);
	void set_click();
	bool getBounds(sf::Vector2f &m);
	cell();
	~cell();
};

