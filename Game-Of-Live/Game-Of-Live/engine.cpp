#include "pch.h"
#include "engine.h"


void engine::move(sf::RenderWindow &window, sf::Font &font)
{
		new_gen();
		render(window, font);
}

void engine::init(unsigned int size_x, unsigned int size_y)
{
	this->size_y = size_y;
	this->size_x = size_x;
	generation = 1;
	grid.resize(this->size_x);
	for (unsigned int i = 0; i < this->size_x; i++)
		grid[i].resize(this->size_y);

	for (unsigned int i = 0; i < this->size_x; i++)
		for (unsigned int j = 0; j < this->size_y; j++)
			grid[i][j].set_position(13*i,12*j);
}


void engine::render(sf::RenderWindow &window, sf::Font &font)
{
	for (unsigned int i = 0; i < size_x; i++) {
		for (unsigned int j = 0; j < size_y; j++) {
			window.draw(grid[i][j]);
		}
	}	
}

void engine::start(sf::RenderWindow & window, sf::Font & font, paterns &patern)
{
	
	const int menu_items = 6;

	sf::Text menu[menu_items];
	sf::Text gen;
	gen.setPosition((window.getSize().x) / 2, 45);
	gen.setFillColor(sf::Color::Black);
	gen.setCharacterSize(15);
	string str[] = { "Play (P)","Stop (S)", "Move back (B)", "Move forward (F)", "Save to RLE (Q)", "Return to menu (ESC)" };
	for (int i = 0; i < menu_items; i++)
	{
		menu[i].setFont(font);
		menu[i].setCharacterSize(15);

		menu[i].setString(str[i]);
		menu[i].setPosition(((window.getSize().x)/ (menu_items + 2)) + i * ((window.getSize().x) / (menu_items + 2)), 15);
	}
	started = true;
	while (started) {
		window.clear(sf::Color::White);
		sf::Event event;
		sf::Vector2f mouse(sf::Mouse::getPosition(window));

		gen.setFont(font);
		gen.setString("Generation " + to_string(generation));
		window.draw(gen);

		while (window.pollEvent(event));
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) || menu[0].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
					play();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || menu[1].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
					stop();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) || menu[2].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
					move_back();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) || menu[3].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
					new_gen();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || menu[4].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
					patern.set_patern(grid2struct(), "new_name.RLE", "new name");
					patern.create_list_of_paterns();
					started = false;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || menu[5].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left)
					started = false;

				if (event.type == sf::Event::Closed) {
					started = false;
				}

				if (paused) {
					for (int x = 0; x < size_x; x++)
					{
						for (int y = 0; y < size_y; y++)
						{
							if (grid[x][y].getBounds(mouse) &&	(event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left))
							{
								grid[x][y].set_click();
								sf::sleep(sf::milliseconds(100));
							}
						}
					}
				}
				else {
					new_gen();
					sf::sleep(sf::milliseconds(50));
				}
			}
			for (int i = 0; i < menu_items; i++)
				if (menu[i].getGlobalBounds().contains(mouse))
					menu[i].setFillColor(sf::Color::Cyan);
				else menu[i].setFillColor(sf::Color::Black);
			for (int i = 0; i < menu_items; i++) 
				window.draw(menu[i]);

			render(window, font);
			window.display();
		}
}

void engine::play()
{
	paused = false;
}

void engine::stop()
{
	paused = true;
}

void engine::move_back()
{
}

void engine::clean_grid()
{
	generation = 1;
	for (unsigned int i = 0; i < size_x; i++)
		for (unsigned int j = 0; j < size_y; j++)
			grid[i][j].set_state(false);
}


patern2load engine::grid2struct() 
{
	patern2load patern;
	int counter_dead, counter_alive, count_x;
	std::string bufor;
	for (unsigned int i = 0; i < size_x; i++) {
		counter_dead = counter_alive = count_x = 0;

		bufor = "";
		for (unsigned int j = 0; j < size_y; j++) {
			if (grid[i][j].get_state()) {
				if (counter_dead > 0) {
					bufor += (counter_dead > 1? std::to_string(counter_dead) : "") + "b";
					counter_dead = 0;
					++ count_x;
				}
				++counter_alive;
				
			}else {
				if (counter_alive > 0) {
					bufor += (counter_alive > 1 ? std::to_string(counter_alive) : "") + "o";
					counter_alive = 0;
					++ count_x;
				}
				++counter_dead;
			}
		}
		if (counter_alive > 0) bufor += std::to_string(counter_alive) + "o";
		if (counter_dead > 0 && counter_dead != size_y) {
			bufor += (counter_dead > 1 ? std::to_string(counter_dead) : "") + "b";
			counter_dead = -1;
		}
		if(bufor.size() > 0) {
			if((count_x - counter_dead) > patern.x)
				patern.x = (count_x - counter_dead);
			bufor += "$";
			patern.patern += bufor;
			patern.y++;
		}
	}
	patern.patern.erase(patern.patern.size()-1);
	patern.patern += "!";
	return patern;
}

void engine::struct2grid(patern2load pat)
{
	if (pat.x+4 > size_y || pat.y+4 > size_x)
		"error";
	else {
		auto pos_o = pat.patern.find("o");
		auto pos_b = pat.patern.find("b");
		auto pos_dol = pat.patern.find("$");
		int x_iterator = 4, y_iterator = 4;

		while (pos_o != std::string::npos || std::string::npos != pos_b || std::string::npos != pos_dol) {
			if (pos_dol < pos_o && pos_dol < pos_b){
				int count = (pos_dol == 0) ? 1 : std::stoi(pat.patern.substr(0, pos_dol));
				y_iterator += count;
				x_iterator = 4;
				pat.patern.erase(0, pos_dol + 1);
			}else if (pos_o < pos_b) {
				int count = (pos_o == 0)? 1 : std::stoi(pat.patern.substr(0, pos_o));
				for (int i = 0; i < count; i++)
					grid[y_iterator][x_iterator++].set_state(true);
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

void engine::new_gen()
{
	generation++;
	auto gridBufor(grid);
	auto checkPosition = [](int y, int size) {return (!( y<0 || y>=size)); };
	auto mod = [](int x, int size) {return (x >= size) ? x - size : ((x<0)? size + x: x); };
	int sumOfNeigh;
	for (unsigned int i = 0; i < size_x; i++) {
		for (unsigned int j = 0; j < size_y; j++) {
			sumOfNeigh = 0;
				for(int ci=-1;ci<2;ci++)
					for(int cj=-1;cj<2;cj++)
						sumOfNeigh +=(ci==0 && cj==0)?0:(int)(checkPosition(j+cj, size_y) && gridBufor[mod(i+ci, size_x)][j+cj].get_state());
				if (sumOfNeigh == 3)
					grid[i][j].set_state(true);
				else if(sumOfNeigh != 2)
					grid[i][j].set_state(false);
		}
	}
}

engine::engine()
{
	started = false;
	paused = true;
}


engine::~engine()
{
}
