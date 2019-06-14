#include <SFML/Graphics.hpp>
#pragma once
class cell : public sf::Drawable, sf::Transformable
{
	bool is_alive;
	bool changed;
public:
	static unsigned int marg;
	static unsigned int size;
	sf::RectangleShape shape;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	bool get_state();
	void set_state(bool);
	void set_changed(bool);
	void set_position(int, int);
	void set_click();
	bool isChanged();
	bool getBounds(sf::Vector2f &m);
	static void setMarg(unsigned int m);
	static void setSize(unsigned int s);
	cell();
	cell(unsigned int, unsigned int);
	~cell();
};

