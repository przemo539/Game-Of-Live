#include "pch.h"
#include "engine.h"


void engine::move()
{
	if (periodMakeMover) {
		newGen();
		render();
	}
}

void engine::init(int size, vector<string> map)
{
	this->size = size;
	bool periodMakeMover = false;
	ready2start = true;
	generation = 1;
	grid.resize(size);
	for (int i = 0; i < size; i++)
		grid[i].resize(size);
	
	string2grid(map);
}

void engine::start()
{
	while (ready2start) {
		render();
		menu();
	}
}

void engine::render()
{
	system("cls");
	auto isAlive = [](bool val) {return((val) ? "X " : "  "); };
	for (int i = 0; i < size; i++)		cout << " _";
	cout << endl;

	for (int i = 0; i < size; i++) {
		cout << "|";
		for (int j = 0; j < size; j++) {
			cout << isAlive(grid[i][j]);
		}
		cout <<"|"<< endl;
	}

	for (int i = 0; i < size; i++)		cout << " -";
	cout << endl;
}

void engine::menu()
{
	cout << "Game menu:" << endl << "1. add alive cell " << endl << "2. del alive cell "<<endl << "3. quit game" << endl <<"4. make 5 move"<<endl<<"5. make a move every period of half sec(enable/disable)" <<endl<<"another key -> 1 move"<<endl<< "Input number:";;
	int menu;
	cin.sync();
	cin >> menu;
	int x, y;
	switch (menu) {
	case 1:
		cout << "Input x(1-" << size << "): ";
		cin >> x;
		cout << "Input y(1-" << size << "): ";
		cin >> y;
		if (x <= size && y <= size)
			grid[y-1][x-1] = true;
		break;
	case 2:
		cout << "Input x(1-"<<size<<"): ";
		cin >> x;
		cout << "Input y(1-" << size << "): ";
		cin >> y;
		if (x <= size && y <= size)
			grid[y-1][x-1] = false;
		break;
	case 3:
		ready2start = false;
		
		break;
	case 4:
		for(int i=0;i<5;i++)
			newGen();
		break;
	case 5:
		makeHalfSecsMove();
		break;
	default:
		newGen();
	}
}

void engine::string2grid(vector <string> map)
{
	if (map.size() != 0 && map.size() < grid.size() && map[0].length() < grid[0].size()) {
		auto returnBoolVal = [](char x) { return (x == '1') ? true : false;  };
		for (unsigned int i = 0; i < map.size(); i++)
			for (unsigned int j = 0; j < map[i].length(); j++)
				grid[i][j] = returnBoolVal(map[i][j]);
	}
}

void engine::makeHalfSecsMove()
{
	if (periodMakeMover) {
		periodMakeMover = false;
	}else {		
		periodMakeMover = true;		
	}	
}
void engine::tick() {
	while (periodMakeMover) {
		newGen();
		render();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}


void engine::newGen()
{
	generation++;
	auto gridBufor(grid);
	auto checkPosition = [](int y, int x, int size) {return (!( y<0 || y>=size)); };
	auto mod = [](int x, int size) {return (x >= size) ? x - size : ((x<0)? size + x: x); };
	int sumOfNeigh;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			sumOfNeigh = 0;
				for(int ci=-1;ci<2;ci++)
					for(int cj=-1;cj<2;cj++)
						sumOfNeigh +=(ci==0 && cj==0)?0:(int)(checkPosition(i+ci, j+cj, size) && gridBufor[i+ci][mod(j+cj, size)]);
				if (sumOfNeigh == 3)
					grid[i][j] = true;
				else if(sumOfNeigh != 2)
					grid[i][j] = false;
		}
	}
}

engine::engine()
{
	ready2start = false;
}


engine::~engine()
{
}
