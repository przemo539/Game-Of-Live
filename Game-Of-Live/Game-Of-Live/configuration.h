#pragma once
#include <boost/program_options.hpp>
#include <fstream>
#include <string>
namespace po = boost::program_options;
class configuration
{
	std::string path;
	int size_x;
	int size_y;
	po::variables_map vm;
	po::options_description fileOptions;
	std::fstream settings_file;
	void read_settings();
public:
	int get_size_x();
	int get_size_y();
	void read();
	void write_settings(const char* name, const char* value);
	configuration(std::string path);
	configuration();
	~configuration();
};

