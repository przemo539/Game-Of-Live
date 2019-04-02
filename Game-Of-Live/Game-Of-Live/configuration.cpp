#include "pch.h"
#include "configuration.h"


void configuration::read_settings()
{
	settings_file.open("settings.ini", std::ifstream::in);
	vm = po::variables_map();
	if (settings_file.is_open()) {
		po::store(po::parse_config_file(settings_file, fileOptions), vm);
		po::notify(vm);
	}
	else {
		//cout << "We have some trouble here.... We got error in parsing config file.." << endl << "App wil be closing";
	}
	settings_file.close();
}

int configuration::get_size_x()
{
	return size_x;
}

int configuration::get_size_y()
{
	return size_y;
}

void configuration::read()
{
	try
	{
		fileOptions.add_options()
			("size_y", po::value<int>(&size_y), "Size y of grid")
			("size_x", po::value<int>(&size_x), "Size x of grid");
		read_settings();
	}
	catch (const po::error &ex)
	{
		//std::cerr << ex.what() << '\n' << "app will be closing" << endl;
	}
}

void configuration::write_settings(const char * name, const char * value)
{
	settings_file.open(path, std::ifstream::out);
	if (settings_file.is_open()) {
		settings_file << name << " = " << value;
	}
	settings_file.close();
}

configuration::configuration()
{
	size_x = 0;
	size_y = 0;
	path = "settings.ini";
}

configuration::configuration(std::string path)
{
	size_x = 0;
	size_y = 0;
	this->path = path;
}


configuration::~configuration()
{
	
}
