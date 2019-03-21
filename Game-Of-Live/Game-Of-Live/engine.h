#pragma once
#include <vector>
#include <iostream>
#include <thread>
using namespace std;


class engine
{
	friend void tick(engine);
	vector<vector<bool>> grid;
	int generation;
	int size;
	bool ready2start;
	bool periodMakeMover;
	void newGen();
	void render();
	void menu();
	void string2grid(vector <string>);
	void makeHalfSecsMove();
	void tick();
public:
	void move();
	void init(int size, vector <string>);
	void start();
	engine();
	~engine();
};

