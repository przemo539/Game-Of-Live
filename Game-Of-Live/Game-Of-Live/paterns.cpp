#include "pch.h"
#include "paterns.h"


void paterns::create_list_of_paterns()
{
	patern.clear();
	name_of_patern.clear();
	try {
		for (const auto & entry : fs::directory_iterator(path)) {
			std::fstream file(entry.path(), std::ios::in);
			std::string data;
			bool finded = false;
			while (!file.eof()) {
				getline(file, data);
				auto found = data.find("#N");
				if (found != std::string::npos) {
					finded = true;
					data.erase(0, found + 3);
					break;
				}
			}
			file.close();
			patern.emplace( ((finded) ? data : fs::path(entry.path()).filename()).string(), entry.path().string() );
			name_of_patern.push_back(((finded) ? data : fs::path(entry.path()).filename()).string());
		}
	}
	catch (const fs::filesystem_error& ex)
	{
		 std::cout << ex.what() << '\n';
	}

}

patern2load paterns::get_patern(std::string name)
{
	patern2load data;
	auto pos = patern.find(name);
	if (name == "!!RANDOM!!!") {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(5, 20);
		std::uniform_int_distribution<> dis2(0, 3);
		std::uniform_int_distribution<> disBool(0, 1);
		data.x = dis(gen);
		data.y = dis(gen);
		std::string paternbuf;
		for (int i = 0; i < data.x; i++)
		{
			for (int j = 0; j< data.y; j++) {
				int num = dis2(gen);
				if(num + j < data.y)
					paternbuf += std::to_string(num) + (disBool(gen)?"b":"o");
				else
					paternbuf += std::to_string(data.y - j) + (disBool(gen) ? "b" : "o");
				j += num;
			}
			paternbuf += "$";
		}
		paternbuf.erase(paternbuf.size() - 1);
		paternbuf += "!";
		data.patern = paternbuf;
	}else if (pos == patern.end()) {
		data.x = data.y = 0;
		data.patern = "!";
	}
	else {
		std::fstream file(pos->second, std::ios::in);
		std::string bufor;
		bool founded = false;
		std::string paternbuf;
		while (!file.eof()) {
			getline(file, bufor);
			if (!founded) {
				auto found_x = bufor.find("x = ");
				auto found_y = bufor.find("y = ");
				if (found_x != std::string::npos && found_y != std::string::npos) {
					data.x = std::stoi(bufor.substr(found_x + 3, found_y - 1));
					data.y = std::stoi(bufor.substr(found_y + 3));
					founded = true;
				}
			}
			else {
				paternbuf += bufor;
			}
		}
		data.patern = paternbuf;
		file.close();
	}
	
	return data;
}

void paterns::set_patern(patern2load pat, std::string filename,  std::string name_of_patern)
{
	//name_of_patern.push_back(name_of_patern);
	std::ofstream file;
	auto temp_path = path;
	temp_path += "/" + filename;
	file.open(temp_path);
	patern.emplace(name_of_patern, temp_path.string());
	
	file << "#N " + name_of_patern << '\n';
	file << "#C This is a " + name_of_patern + "." << '\n';
	file << "x = " + std::to_string(pat.x) + ", y = " + std::to_string(pat.y) << '\n';
	file << pat.patern;
	file.close();
}

std::vector<std::string> paterns::get_list_paterns()
{
	return name_of_patern;
}

paterns::paterns()
{
	path = fs::current_path();
	path += "/RLE";
}

paterns::paterns(fs::path path)
{
	this->path = path;

}



paterns::~paterns()
{
}
