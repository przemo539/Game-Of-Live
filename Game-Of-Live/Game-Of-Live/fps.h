#pragma once
#include <string>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
class fps
{
	sf::Clock clock;
public:
	fps();
	~fps();
	std::string getFPS();
};
