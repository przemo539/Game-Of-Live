#include "pch.h"
#include "engine.h"


void engine::init(int size)
{
	this->size = size;
	ready2start = true;
	generation = 1;
	grid.resize(size);
	for (int i = 0; i < size; i++)
		grid[i].resize(size);
	

	grid[1][1] = 1;
	grid[2][1] = 1;
	grid[3][1] = 1;
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
	cout << "Game menu:" << endl << "1. add alive cell " << endl << "2. del alive cell "<<endl << "3. quit game" << endl <<"4. make 5 move"<<endl<<"another key -> 1 move"<<endl<< "Input number:";;
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
			grid[x-1][y-1] = true;
		break;
	case 2:
		cout << "Input x(1-"<<size<<"): ";
		cin >> x;
		cout << "Input y(1-" << size << "): ";
		cin >> y;
		if (x <= size && y <= size)
			grid[x-1][y-1] = false;
		break;
	case 3:
		ready2start = false;
		
		break;
	case 4:
		for(int i=0;i<5;i++)
			newGen();
		break;
	default:
		newGen();
	}
}

void engine::newGen()
{
	generation++;
	auto gridBufor(grid);
	auto checkPosition = [](int x, int y, int size) {return (!( x<0 || y<0 || x >= size || y>=size)); };
	int sumOfNeigh;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			sumOfNeigh = 0;
				for(int ci=-1;ci<2;ci++)
					for(int cj=-1;cj<2;cj++)
						sumOfNeigh +=(ci==0 && cj==0)?0:(int)(checkPosition(i+ci, j+cj, size) && gridBufor[i+ci][j+cj]);
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
