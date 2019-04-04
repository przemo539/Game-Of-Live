#pragma once
#include <filesystem>
#include <fstream>
#include <map> 
#include <string>
#include <iostream>

#include "patern2load.h"

namespace fs = std::filesystem;
class paterns
{
	fs::path path;
	std::map<std::string, std::string> patern;
	std::vector <std::string> name_of_patern;
public:
	void create_list_of_paterns();
	patern2load get_patern(std::string name);
	void set_patern(patern2load pat, std::string filename, std::string name_of_patern);
	std::vector <std::string> get_list_paterns();
	paterns();
	paterns(fs::path);
	~paterns();
};

