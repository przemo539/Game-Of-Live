#include "pch.h"
#include "engine.h"
#include "configuration.h"
#include "paterns.h"
#include <iostream>
#include <string>
#include <thread>




void call(engine *game) {
	while (1) {
		game->move();
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
}

int main()
{


	
	configuration conf;
	conf.read();
	

	engine game;
	thread th(call, &game);
	th.joinable();
	game.init(conf.get_size_x(), conf.get_size_y());
	game.start();
	th.join();	

}
