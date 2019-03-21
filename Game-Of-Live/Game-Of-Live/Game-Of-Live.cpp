#include "pch.h"
#include "engine.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <fstream>
#include <string>
#include <thread>


namespace po = boost::program_options;

void read_settings(po::options_description& fileOptions,
	po::variables_map& vm)
{
	std::ifstream settings_file("settings.ini");

	// Clear the map.
	vm = po::variables_map();
	if(settings_file.is_open()) {
		po::store(po::parse_config_file(settings_file, fileOptions), vm);
		po::notify(vm);
	} else {
		cout << "We have some trouble here.... We got error in parsing config file.." << endl << "App wil be closing";
	}
}

void write_settings(const char* name, const char* value)
{
	std::ofstream settings_file("settings.ini");
	if (settings_file.is_open()) {
		settings_file << name << " = " << value;
	}
}

void call(engine *game) {
	while (1) {
		game->move();
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

int main()
{
	int size = 0;
	vector<std::string> glider;
	vector<std::string> gliderGun;
	try
	{
		po::options_description fileOptions{ "File" };
		fileOptions.add_options()
			("glider", po::value<vector<std::string>>(&glider)->multitoken()->composing(), "Struct of Glider on grid")
			("gliderGun", po::value<vector<std::string>>(&gliderGun)->multitoken()->composing(), "Struct of GliderGun on grid")
			("size", po::value<int>(&size), "Size of grid");
		po::variables_map vm;


		read_settings(fileOptions, vm);
	}
	catch (const po::error &ex)
	{
		std::cerr << ex.what() << '\n' << "app will be closing" << endl;
	}
	if (size > 0) {
		int menu = 0;
		if (!glider.empty() || !gliderGun.empty()) {			
			cout << "Starting menu: "  << endl << (!glider.empty() ? "1. load Glider" : " ") << endl << (!gliderGun.empty() ? "2. load Gosper glider gun" : "") << endl << "Type number: ";
			cin.sync();
			cin >> menu;

		}
	
		system("cls");
		
		engine game;
		thread th(call, &game);
		th.joinable();
		game.init(size, ((menu == 1) ? glider : ((menu == 2) ? gliderGun : (vector<string>)(NULL))));
		game.start();
		th.join();
	}
}
