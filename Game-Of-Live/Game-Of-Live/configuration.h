#pragma once
#include <boost/program_options.hpp>
#include <fstream>
#include <string>
namespace po = boost::program_options;
class configuration
{
	std::string path;
	unsigned int size_x;
	unsigned int size_y;
	unsigned int h_resolution;
	unsigned int v_resolution;
	po::variables_map vm;
	po::options_description fileOptions;
	std::fstream settings_file;
	void read_settings();
public:
	unsigned int get_size_x();
	unsigned int get_size_y();
	unsigned int get_h_resolution();
	unsigned int get_v_resolution();
	void read();
	void write_settings(const char* name, const char* value);
	configuration(std::string path);
	configuration();
	~configuration();
};

