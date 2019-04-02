#pragma once
#include <filesystem>
#include <fstream>
#include <map> 
#include <string>
#include <iostream>

struct patern2load {
	int x = 0, y=0;
	std::string patern;
};

namespace fs = std::filesystem;
class paterns
{
	fs::path path;
	std::map<std::string, std::string> patern;
public:
	void create_list_of_paterns();
	patern2load get_patern(std::string name);
	void set_patern(patern2load pat, std::string filename, std::string name_of_patern);
	paterns();
	paterns(fs::path);
	~paterns();
};

