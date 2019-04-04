#include "pch.h"
#include "engine.h"


void engine::move()
{
	if (periodMakeMover) {
		newGen();
		render();
	}
}

void engine::init(int size_x, int size_y)
{
	this->size_x = size_x;
	this->size_y = size_y;
	bool periodMakeMover = false;
	ready2start = true;
	generation = 1;
	grid.resize(size_y);
	for (int i = 0; i < size_y; i++)
		grid[i].resize(size_x);
	
	struct2grid(patern.get_patern("Queen bee shuttle"));
	grid2struct();
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
	for (int i = 0; i < size_x; i++)		cout << " _";
	cout << endl;

	for (int i = 0; i < size_y; i++) {
		cout << "|";
		for (int j = 0; j < size_x; j++) {
			cout << isAlive(grid[i][j]);
		}
		cout <<"|"<< endl;
	}

	for (int i = 0; i < size_x; i++)		cout << " -";
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
		cout << "Input x(1-" << size_x << "): ";
		cin >> x;
		cout << "Input y(1-" << size_y << "): ";
		cin >> y;
		if (x <= size_x && y <= size_y)
			grid[y-1][x-1] = true;
		break;
	case 2:
		cout << "Input x(1-"<<size_x<<"): ";
		cin >> x;
		cout << "Input y(1-" << size_y << "): ";
		cin >> y;
		if (x <= size_x && y <= size_y)
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

void engine::grid2struct() 
{
	patern2load pat;
	int counter_dead, counter_alive;
	std::string bufor;
	for (int i = 0; i < size_y; i++) {
		counter_dead = counter_alive = 0;
		bufor = "";
		for (int j = 0; j < size_x; j++) {
			if (grid[i][j]) {
				if (counter_dead > 0) {
					bufor += (counter_dead > 1? std::to_string(counter_dead) : "") + "b";
					counter_dead = 0;
					
				}
				++counter_alive;
				
			}else {
				if (counter_alive > 0) {
					bufor += (counter_alive > 1 ? std::to_string(counter_alive) : "") + "o";
					counter_alive = 0;
				}
				++counter_dead;
			}
		}
		if (counter_alive > 0) bufor += std::to_string(counter_alive) + "o";
		if (counter_dead > 0 && counter_dead != size_x)  bufor += (counter_dead > 1 ? std::to_string(counter_dead) : "") + "b";
		if(bufor.size() > 0) {
			bufor += "$";
			pat.patern += bufor;
			pat.y++;
		}
	}
	pat.patern.erase(pat.patern.size()-1);
	pat.patern += "!";
	pat.x = size_x;
	patern.set_patern(pat, "nowy.rle", "testowy");
}

void engine::struct2grid(patern2load pat)
{
	if (pat.x > size_x || pat.y > size_y)
		"error";
	else {
		auto pos_o = pat.patern.find("o");
		auto pos_b = pat.patern.find("b");
		auto pos_dol = pat.patern.find("$");
		int x_iterator = 0, y_iterator = 0;

		while (pos_o != std::string::npos || std::string::npos != pos_b || std::string::npos != pos_dol) {
			if (pos_dol < pos_o && pos_dol < pos_b){
				int count = (pos_dol == 0) ? 1 : std::stoi(pat.patern.substr(0, pos_dol));
				y_iterator += count;
				x_iterator = 0;
				pat.patern.erase(0, pos_dol + 1);
			}else if (pos_o < pos_b) {
				int count = (pos_o == 0)? 1 : std::stoi(pat.patern.substr(0, pos_o));
				for (int i = 0; i < count; i++)
					grid[y_iterator][x_iterator++] = true;
				pat.patern.erase(0, pos_o+1);
			}else {
				int count = (pos_b == 0)? 1 : std::stoi(pat.patern.substr(0, pos_b));
				x_iterator += count;
					
				pat.patern.erase(0, pos_b + 1);
			}
			 pos_o = pat.patern.find("o");
			 pos_b = pat.patern.find("b");
			 pos_dol = pat.patern.find("$");
		}


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
	for (int i = 0; i < size_y; i++) {
		for (int j = 0; j < size_x; j++) {
			sumOfNeigh = 0;
				for(int ci=-1;ci<2;ci++)
					for(int cj=-1;cj<2;cj++)
						sumOfNeigh +=(ci==0 && cj==0)?0:(int)(checkPosition(i+ci, j+cj, size_y) && gridBufor[i+ci][mod(j+cj, size_x)]);
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
	patern.create_list_of_paterns();
}


engine::~engine()
{
}
