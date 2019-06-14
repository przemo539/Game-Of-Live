#include "pch.h"
#include "fps.h"


fps::fps()
{
}

fps::~fps()
{
}

std::string fps::getFPS()
{	

	sf::Time time = clock.getElapsedTime(c);
	clock.restart().asSeconds();
	return std::to_string(round(1.0f / time.asSeconds()));
}

