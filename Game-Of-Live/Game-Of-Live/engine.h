#pragma once
#include <vector>
#include <iostream>
using namespace std;
class engine
{
	vector<vector<bool>> grid;
	int generation;
	int size;
	bool ready2start;
	void newGen();
	void render();
	void menu();
public:
	void init(int size);
	void start();
	engine();
	~engine();
};

