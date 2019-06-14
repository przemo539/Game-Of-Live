#include "pch.h"
#include "game.h"


void game::start()
{
	while (state != END)
	{
		switch (state)
		{
		case MENU:
			menu();
			break;
		case GAME:
			render_grid();
			break;
		case LOAD_RLE:
			list_rle();
			break;	
		case SETTINGS:
			settings();
			break;
		}
	}
	end();
}

game::game()
{
	game_name = "GAME OF LIFE";
	conf.read();
	patern.create_list_of_paterns();

	ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(VideoMode(conf.get_h_resolution(), conf.get_v_resolution()), game_name, Style::Default, settings);
	window.setFramerateLimit(10);
	if (!font.loadFromFile("arial.ttf"))
	{
		//MessageBoxA(NULL, "Font not found!", "ERROR", NULL);
		return;
	}

	Text title("LOADING ...", font, 80);
	title.setStyle(Text::Bold);
	title.setFillColor(Color::Black);

	title.setPosition(conf.get_h_resolution()  / 2 - title.getGlobalBounds().width / 2, conf.get_v_resolution() / 2 - title.getGlobalBounds().height / 2);

	window.clear(Color::White);
	window.draw(title);
	window.display();
	engin.init(conf.get_size_x(), conf.get_size_y(), conf.get_h_resolution(), conf.get_v_resolution(), window, font);
	state = MENU;
}


game::~game()
{
}


void game::menu()
{
	Text title(game_name, font, 80);
	title.setStyle(Text::Bold);
	title.setFillColor(Color::Black);

	title.setPosition(conf.get_h_resolution() / 2 - title.getGlobalBounds().width / 2, 20);

	const int menu_items = 4;

	Text menu[menu_items];

	string str[] = { "Play", "LOAD FROM RLE", "SETTINGS", "Exit" };
	for (int i = 0; i < menu_items; i++)
	{
		menu[i].setFont(font);
		menu[i].setCharacterSize(65);

		menu[i].setString(str[i]);
		menu[i].setPosition((float)conf.get_h_resolution() / 2 - menu[i].getGlobalBounds().width / 2, (float)250 + i * 120);
	}

	while (state == MENU)
	{
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while (window.pollEvent(event))
		{

			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
				state = END;


			else if (menu[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				engin.cleanGrid();
				state = GAME;
			}


			else if (menu[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = LOAD_RLE;
			}

			else if (menu[2].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = SETTINGS;
			} if (menu[3].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				state = END;
			}
			if (event.type == Event::Resized) {
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));

			}
		}
		for (int i = 0; i < menu_items; i++)
			if (menu[i].getGlobalBounds().contains(mouse))
				menu[i].setFillColor(Color::Cyan);
			else menu[i].setFillColor(Color::Black);

		window.clear(Color::White);

		window.draw(title);
		for (int i = 0; i < menu_items; i++)
			window.draw(menu[i]);

		window.display();
	}
}

void game::render_grid()
{
	engin.start(patern);
	state = MENU;
}

void game::list_rle()
{
	vector<string> list_paterns = patern.get_list_paterns();
	list_paterns.push_back("!!RANDOM!!!");
	auto menu = std::make_unique<Text[]>(list_paterns.size()+1);
	for(unsigned int i=0; i< (unsigned int)list_paterns.size(); i++)
	{
		menu[i].setFont(font);
		menu[i].setCharacterSize(20);

		menu[i].setString(list_paterns[i]);
		menu[i].setPosition((float)conf.get_h_resolution() / 2 - menu[i].getGlobalBounds().width / 2,(float) 30 + i * 30);
	}
	while (state == LOAD_RLE) {

		Vector2f mouse(Mouse::getPosition(window));
		Event event;
		while (window.pollEvent(event))
		{
			for (unsigned int i = 0; i < (unsigned int)list_paterns.size(); i++)
				if (menu[i].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
				{
					engin.cleanGrid();
					engin.struct2grid(patern.get_patern(list_paterns[i]));
					state = GAME;
				}
			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
				state = END;
		}

		for (unsigned int i = 0; i < (unsigned int)list_paterns.size(); i++)
			if (menu[i].getGlobalBounds().contains(mouse))
				menu[i].setFillColor(Color::Cyan);
		else menu[i].setFillColor(Color::Black);

		window.clear(Color::White);

		for (unsigned int i = 0; i < (unsigned int)list_paterns.size(); i++)
			window.draw(menu[i]);

		window.display();
	}
}

void game::end()
{
	Text title("Releasing resources in progress...", font, 80);
	title.setStyle(Text::Bold);
	title.setFillColor(Color::Black);

	title.setPosition(conf.get_h_resolution() / 2 - title.getGlobalBounds().width / 2, conf.get_v_resolution() / 2 - title.getGlobalBounds().height / 2);

	window.clear(Color::White);
	window.draw(title);
	window.display();
}

void game::settings()
{
	
	Text title("SETTINGS", font, 80);
	title.setStyle(Text::Bold);
	title.setFillColor(Color::Black);

	title.setPosition(conf.get_h_resolution() / 2 - title.getGlobalBounds().width / 2, 20);
	Text label[2];
	label[0].setString("X: ");
	label[1].setString("Y: ");
	Text size[2];
	size[0].setString(to_string(conf.get_size_x()));
	size[1].setString(to_string(conf.get_size_y()));
	for (int i = 0; i < 2; i++) {
		size[i].setFont(font);
		size[i].setFillColor(Color::Black);
		size[i].setCharacterSize(65);
		size[i].setPosition((float)conf.get_h_resolution() / 2, (float)250 + i * 120);

		label[i].setFont(font);
		label[i].setCharacterSize(65);		
		label[i].setFillColor(Color::Black);
		label[i].setPosition((float)conf.get_h_resolution() / 2 -100, (float)250 + i * 120);
	}
	enum settingStates { X, Y, NULLS};

	settingStates settingstate = NULLS;
	while (state == SETTINGS)
	{
		window.clear(sf::Color::White);
		window.draw(title);
		Vector2f mouse(Mouse::getPosition(window));
		Event event;

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape) {
				int x = stoi(size[0].getString().toAnsiString());
				int y = stoi(size[1].getString().toAnsiString());
					engin.changeGrid(x, y, conf.get_h_resolution(), conf.get_v_resolution());
				state = MENU;
			}

			if (size[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left || label[0].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				settingstate = X;
			}
			else if (size[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left || label[1].getGlobalBounds().contains(mouse) &&
				event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
			{
				settingstate = Y;
			}
			else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left) {
				settingstate = NULLS;
			}

			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 8 || event.text.unicode >= 48 && event.text.unicode <= 57 )
				{
					if (settingstate == X) {
						sf::String bufor = size[0].getString();
						if (event.text.unicode == '\b' && bufor.getSize()>0)
							bufor.erase(bufor.getSize()-1, 1);
						else
							bufor += event.text.unicode;
						size[0].setString(bufor);
					}
					if (settingstate == Y) {
						sf::String bufor = size[1].getString();
						if (event.text.unicode == '\b' && bufor.getSize() > 0)
							bufor.erase(bufor.getSize() - 1, 1);
						else
							bufor += event.text.unicode;
						size[1].setString(bufor);
					}
				}
			}

			for (int i = 0; i < 2; i++)
				if (size[i].getGlobalBounds().contains(mouse) || label[i].getGlobalBounds().contains(mouse))
				{
					size[i].setFillColor(Color::Cyan);
					label[i].setFillColor(Color::Cyan);
				}
				else {
					size[i].setFillColor(Color::Black);
					label[i].setFillColor(Color::Black);
				}
			if (settingstate == X) {
				size[0].setFillColor(Color::Red);
				label[0].setFillColor(Color::Red);
			}
			if (settingstate == Y) {
				size[1].setFillColor(Color::Red);
				label[1].setFillColor(Color::Red);
			}

		}
		for (int i = 0; i < 2; i++) {
			window.draw(label[i]);
			window.draw(size[i]);
		}
		window.display();
	}
}

