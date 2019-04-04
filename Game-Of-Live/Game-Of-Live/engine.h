#pragma once
#include <vector>
#include <iostream>
#include <thread>
#include <string>
#include "paterns.h"
using namespace std;


class engine
{
	friend void tick(engine);
	vector<vector<bool>> grid;
	int generation;
	int size_x;
	int size_y;
	bool ready2start;
	bool periodMakeMover;
	paterns patern;
	void newGen();
	void render();
	void menu();
	void struct2grid(patern2load);
	void grid2struct();
	void makeHalfSecsMove();
	void tick();
public:
	void move();
	void init(int size_x, int size_y);
	void start();
	engine();
	~engine();
};

