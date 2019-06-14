#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include "patern2load.h"
#include "cell.h"
#include "fps.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "paterns.h"
using namespace std;


class engine
{
	sf::Clock clock;
	sf::RenderWindow *window;
	sf::Font *font;
	list < vector<vector<cell>> > moveVect;
	std::list < vector<vector<cell>>>::iterator current;
	std::list < vector<vector<cell>>>::iterator newOne;

	unsigned int generation;
	unsigned int maxGeneration;
	unsigned int sizeY;
	unsigned int sizeX;
	bool paused;
	bool started;
	bool firstRender;
	bool firstAloc;
	unsigned int threadCount;
	void newGen(unsigned int threadIndex);
	void createGrid(unsigned int threadIndex);
	float getFPS(const sf::Time& time);
public:
	void struct2grid(patern2load);
	patern2load grid2struct();
	void move();
	void init(unsigned int size_x, unsigned int size_y, int x_res, int y_res, sf::RenderWindow& , sf::Font&);
	void render();
	void fullRender();
	void start(paterns &patern);
	void play();
	void stop();
	void moveBack();
	void cleanGrid();
	void changeGrid(int x, int y, int x_res, int y_res);
	
	engine();
	~engine();
};

