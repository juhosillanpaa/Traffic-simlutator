#include <iostream>
#include "Engine/Engine.hpp"

int main (int argc, char* argv[]) {

	if (argc == 1) {
		Engine::Engine simulation;
		simulation.construct_sample_city();
		simulation.simulate();
	} else if (argc == 2) {
		Engine::Engine(argv[1]);
	} else {
		std::cout << "Too many arguments! Only pass the file name of your city file." << std::endl;
	}

}