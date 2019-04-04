#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "patern2load.h"
#include "cell.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "paterns.h"
using namespace std;


class engine
{
	vector<vector<cell>> grid;
	unsigned int generation;
	unsigned int size_y;
	unsigned int size_x;
	bool paused;
	bool started;
	void new_gen();

public:
	void struct2grid(patern2load);
	patern2load grid2struct();
	void move(sf::RenderWindow &window, sf::Font &font);
	void init(unsigned int size_x, unsigned int size_y);
	void render(sf::RenderWindow &window, sf::Font &font);
	void start(sf::RenderWindow &window, sf::Font &font, paterns &patern);
	void play();
	void stop();
	void move_back();
	void clean_grid();
	engine();
	~engine();
};

