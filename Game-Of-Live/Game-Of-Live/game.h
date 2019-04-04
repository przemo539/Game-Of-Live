#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "configuration.h"
#include "engine.h"
#include "paterns.h"

using namespace sf;
using namespace std;

class game
{
	string game_name;
	RenderWindow window;
	paterns patern;
	configuration conf;
	engine engin;
	Font font;
	enum GameState { MENU, GAME, LOAD_RLE, END };
	GameState state;
	void menu();
	void render_grid();
	void list_rle();
public:
	void start();
	game();
	~game();
};
