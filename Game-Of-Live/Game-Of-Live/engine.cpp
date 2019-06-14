#include "pch.h"
#include "engine.h"


void engine::move()
{
	if (generation == maxGeneration) {
		std::vector<std::thread> threads;
		for (unsigned int i = 0; i < threadCount; i++)
			threads.push_back(std::thread(&engine::newGen, this, i));

		for (auto& thread : threads) {
			thread.join();
		}
		threads.clear();
		moveVect.push_back(vector<vector<cell>>(this->sizeX));
		for (unsigned int i = 0; i < threadCount; i++)
			threads.push_back(std::thread(&engine::createGrid, this, i));

		for (auto& thread : threads) {
			thread.join();
		}
		if (moveVect.size() > 2) {
			++newOne;
			++current;
		}
		++maxGeneration;
	}
	else
	{
		++current;
	}
	++generation;
}

void engine::init(unsigned int size_x, unsigned int size_y, int x_res, int y_res, sf::RenderWindow& windows, sf::Font &fonts)
{
	window = &windows;
	font = &fonts;
	threadCount = std::thread::hardware_concurrency();
	cout << "oblusujemy max: " << threadCount << "watkow" << endl;;
	int size = (x_res / size_x < (y_res - 120) / size_y) ? x_res / size_x : (y_res - 120) / size_y;
	size = (size < 4) ? 4 : size;
	cell::setSize(size);
	this->sizeY = (size_y*(size+1) > (y_res - 120)? (y_res - 120) / (size+1):size_y);
	this->sizeX = (size_x*(size + 1) > (x_res-100) ? (x_res - 200) / (size + 1) : size_x);
	cell::setMarg((x_res - 60-(size+1) * this->sizeX) / 2);
	generation = 1;
	maxGeneration = 1;
	std::vector<std::thread> threads;
	for (int i = 0; i < 2; i++)
		threads.push_back(std::thread(&engine::createGrid, this, i));

	for (auto& thread : threads) {
		thread.join();
	}
	current = moveVect.begin();
	newOne = moveVect.begin();
	++newOne;
}

void engine::fullRender()
{
	sf::RectangleShape clearCell;
	clearCell.setSize(sf::Vector2f(10, 10));
	vector< vector<cell> >::iterator row;
	vector<cell>::iterator col;
	for(int i = 0; i < sizeX; i++){
		for (int j = 0; j < sizeY; j++) {
				window->draw((*current)[i][j]);
		}
	}
}


void engine::render()
{
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if ((*current)[i][j].isChanged()) {
				window->draw((*current)[i][j]);
			}			
		}
	}
}

void engine::start(paterns &patern)
{
	window->clear(sf::Color::White);
	firstRender = true;
	sf::RectangleShape clearFPS;
	clearFPS.setSize(sf::Vector2f(100, 50));

	sf::RectangleShape clearMenu;
	clearMenu.setSize(sf::Vector2f(150, 30));

	sf::RectangleShape clearGen;
	clearGen.setSize(sf::Vector2f(150, 30));
	clearGen.setPosition((window->getSize().x / (float)2) - 10, (float)35);
	const int menu_items = 6;

	
	sf::Text menu[menu_items];
	sf::Text gen;
	gen.setPosition((window->getSize().x) / (float)2, (float)45);
	gen.setFillColor(sf::Color::Black);
	gen.setFont(*font);
	gen.setCharacterSize(15);



	sf::Text FPScounter;
	FPScounter.setFont(*font);
	FPScounter.setCharacterSize(15);
	FPScounter.setFillColor(sf::Color::Black);

	string str[] = { "Play (P)","Stop (S)", "Move back (B)", "Move forward (F)", "Save to RLE (Q)", "Return to menu (ESC)" };
	for (int i = 0; i < menu_items; i++)
	{
		menu[i].setFont(*font);
		menu[i].setCharacterSize(15);

		menu[i].setString(str[i]);
		menu[i].setPosition((float)((window->getSize().x)/ (menu_items + 2)) + i * ((window->getSize().x) / (menu_items + 2)), (float)15);
		menu[i].setFillColor(sf::Color::Black);
		window->draw(menu[i]);
	}



	started = true;
	while (started) {
		
		sf::Event event;
		sf::Vector2f mouse(sf::Mouse::getPosition(*window));


		gen.setString("Generation " + to_string(generation));
		window->draw(clearGen);
		window->draw(gen);

		while (window->pollEvent(event))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) || menu[0].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
					play();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || menu[1].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
					stop();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) || menu[2].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
					moveBack();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) || menu[3].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
					move();

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || menu[4].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
					patern.set_patern(grid2struct(), "new_name.RLE", "new name");
					patern.create_list_of_paterns();
					started = false;
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) || menu[5].getGlobalBounds().contains(mouse) &&
					event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
					started = false;

				if (event.type == sf::Event::Closed) {
					started = false;
				}

				if (paused) {
					for (int i = 0; i < sizeX; i++) {
						for (int j = 0; j < sizeY; j++) {
							if ((event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) && (*current)[i][j].getBounds(mouse)) {
								(*current)[i][j].set_click();
								if (generation > 1) {
									--current;
									(*current)[i][j].set_changed(true);
									++current;
								}
							}
						}
					}
				}

				for (int i = 0; i < menu_items; i++)
					if (menu[i].getFillColor() == sf::Color::Cyan) {
						clearMenu.setPosition(menu[i].getPosition());
						window->draw(clearMenu);
						menu[i].setFillColor(sf::Color::Black);
						window->draw(menu[i]);
					}else if (menu[i].getGlobalBounds().contains(mouse)) {
						menu[i].setFillColor(sf::Color::Cyan);
						window->draw(menu[i]);
					}
					
			
		}
		
		if (!paused) {
			move();
		}

			if (firstRender) {
				fullRender();
				firstRender = false;
			}
			else {
				render();
				//(window, font);
			}
			window->draw(clearFPS);
			FPScounter.setString(std::to_string(getFPS(clock.restart())));
			window->draw(FPScounter);
			window->display();
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

void engine::moveBack()
{
	if (generation > 1) {
		--current;
		--generation;
		if (generation == 1)
			firstRender = true;
	}
}

void engine::cleanGrid()
{
	if (!firstAloc) {
		generation = 1;
		maxGeneration = 1;

		moveVect.clear();
		createGrid(0);
		createGrid(0);

		cout << moveVect.size();
		current = moveVect.begin();
		newOne = std::next(moveVect.begin(), 1);
	}
	firstAloc = false;
}

void engine::changeGrid(int x, int y, int x_res, int y_res)
{
	int size = (x_res / x < (y_res - 120) / y) ? x_res / x : (y_res - 120) / y;
	size = (size < 4) ? 4 : size;
	cell::setSize(size);
	this->sizeY = (y*(size + 1) > (y_res - 120) ? (y_res - 120) / (size + 1) : y);
	this->sizeX = (x*(size + 1) > (x_res - 100) ? (x_res - 200) / (size + 1) : x);
	cell::setMarg((x_res - 60 - (size + 1) * this->sizeX) / 2);
	firstAloc = false;
}


patern2load engine::grid2struct() 
{
	patern2load patern;
	unsigned int counter_dead, counter_alive, x_start, y_start;
	int x_last, y_last;
	x_start = sizeX+1;
	y_start = sizeY+1;
	x_last = y_last = -2;
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			if ((*current)[i][j].get_state() ){
				if (x_start > i) x_start = i;
				if (y_start > j) y_start = j;
				if (x_last < i+1) x_last = i+1;
				if (y_last < j+1) y_last = j+1;
			}
		}
	}


	std::string bufor;
	for (int i = x_start; i < x_last; i++) {
		counter_dead = counter_alive = 0;

		bufor = "";
		for (int j = y_start; j < y_last; j++) {
			if ((*current)[i][j].get_state()) {
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
		if (counter_dead > 0 && counter_dead != sizeY) {
			bufor += (counter_dead > 1 ? std::to_string(counter_dead) : "") + "b";
			counter_dead = -1;
		}
			bufor += "$";
			patern.patern += bufor;
	}
		patern.x = x_last - x_start;
		patern.y = y_last - y_start;
	patern.patern.erase(patern.patern.size()-1);
	patern.patern += "!";
	return patern;
}

void engine::struct2grid(patern2load pat)
{

	if (pat.x > sizeY || pat.y > sizeX)
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
					(*current)[y_iterator][x_iterator++].set_state(true);
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

void engine::newGen(unsigned int threadIndex)
{
	cout << "newgen start nr: " << threadIndex << endl;
	auto checkPosition = [](int y, int size) {return (!( y<0 || y>=size)); };
	auto mod = [](int x, int size) {return (x >= size) ? x - size : ((x<0)? size + x: x); };
	int sumOfNeigh;
	int x_size = sizeX / threadCount;
	int rest = 0;
	if (threadIndex + 1 == threadCount) {
		rest = sizeX - (threadCount*x_size);
	}
	for (unsigned int i = threadIndex* x_size; i < (threadIndex+1)*x_size+rest; i++) {
		for (unsigned int j = 0; j < sizeY; j++) {
			sumOfNeigh = 0;
			for (int ci = -1; ci < 2; ci++)
				for (int cj = -1; cj < 2; cj++)
					//sumOfNeigh += (ci == 0 && cj == 0) ? 0 : (int)(checkPosition(j + cj, sizeY) && (*current)[mod(i + ci, sizeX)][j + cj].get_state());
					sumOfNeigh += (ci == 0 && cj == 0) ? 0 : (int)((*current)[mod(i + ci, sizeX)][mod(j + cj, sizeY)].get_state());
			if (sumOfNeigh == 2 && (*current)[i][j].get_state()) {
				(*newOne)[i][j].set_state(true);
				(*current)[i][j].set_changed(true);
			}
			else if (sumOfNeigh == 3)
			{
				(*newOne)[i][j].set_state(true);
				(*current)[i][j].set_changed(true);
			}
			else if (sumOfNeigh != 2 && (*current)[i][j].get_state())
			{			
				(*newOne)[i][j].set_state(false);
				(*current)[i][j].set_changed(true);
			}
		}
	}
	cout << "newgen end nr: " << threadIndex<< endl;
}

void engine::createGrid(unsigned int threadIndex)
{
	int Xsize;
	int rest = 0;
	std::list < vector<vector<cell>>>::iterator grid;
	if (moveVect.size() <= 2) {
		cout << "create start";
		moveVect.push_back(vector<vector<cell>>(this->sizeX));
		Xsize = sizeX;
		threadIndex = 0;

		grid = std::next(moveVect.end(), -1);
	}
	else {
		Xsize = sizeX / threadCount;
		if (threadIndex + 1 == threadCount) {
			rest = sizeX - (threadCount*Xsize);
		}
		grid = std::next(moveVect.end(), -1);
	}

	for (unsigned int i = threadIndex * Xsize; i < (threadIndex + 1)*Xsize+rest; i++) {
		(*grid)[i].resize(this->sizeY);
		for (unsigned int y = 0; y < this->sizeY; y++)
			(*grid)[i][y].set_position(i, y);
	}
	
	cout << "utworzono" << endl;
	

}

float engine::getFPS(const sf::Time & time)
{
	return (1000000.0f / time.asMicroseconds());
}

engine::engine()
{
	started = false;
	paused = true;
	firstAloc = true;
}


engine::~engine()
{
}
