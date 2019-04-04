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
		}
	}
}

game::game()
{
	game_name = "GAME OF LIFE";
	conf.read();
	patern.create_list_of_paterns();

	ContextSettings settings;
	settings.antialiasingLevel = 8;

	window.create(VideoMode(conf.get_h_resolution(), conf.get_v_resolution()), game_name, Style::Default, settings);
	window.setFramerateLimit(60);
	if (!font.loadFromFile("arial.ttf"))
	{
		//MessageBoxA(NULL, "Font not found!", "ERROR", NULL);
		return;
	}
	engin.init(conf.get_size_x(), conf.get_size_y());
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

	const int menu_items = 3;

	Text menu[menu_items];

	string str[] = { "Play", "Load from RLE", "Exit" };
	for (int i = 0; i < menu_items; i++)
	{
		menu[i].setFont(font);
		menu[i].setCharacterSize(65);

		menu[i].setString(str[i]);
		menu[i].setPosition(conf.get_h_resolution() / 2 - menu[i].getGlobalBounds().width / 2, 250 + i * 120);
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
				engin.clean_grid();
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
	engin.start(window, font, patern);
	state = MENU;
}

void game::list_rle()
{
	vector<string> list_paterns = patern.get_list_paterns();
	auto menu = std::make_unique<Text[]>(list_paterns.size());
	for(int i=0; i< list_paterns.size(); i++)
	{
		menu[i].setFont(font);
		menu[i].setCharacterSize(20);

		menu[i].setString(list_paterns[i]);
		menu[i].setPosition(conf.get_h_resolution() / 2 - menu[i].getGlobalBounds().width / 2, 30 + i * 30);
	}
	while (state == LOAD_RLE) {

		Vector2f mouse(Mouse::getPosition(window));
		Event event;
		while (window.pollEvent(event))
		{
			for (int i = 0; i < list_paterns.size(); i++)
				if (menu[i].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left)
				{
					engin.clean_grid();
					engin.struct2grid(patern.get_patern(list_paterns[i]));
					state = GAME;
				}
			if (event.type == Event::Closed || event.type == Event::KeyPressed &&
				event.key.code == Keyboard::Escape)
				state = END;
		}

		for (int i = 0; i < list_paterns.size(); i++)
			if (menu[i].getGlobalBounds().contains(mouse))
				menu[i].setFillColor(Color::Cyan);
		else menu[i].setFillColor(Color::Black);

		window.clear(Color::White);

		for (int i = 0; i < list_paterns.size(); i++)
			window.draw(menu[i]);

		window.display();
	}
}

