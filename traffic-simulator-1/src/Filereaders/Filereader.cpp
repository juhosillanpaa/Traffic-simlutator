#include "data.json"
#include <iostream>
#include <fstream>
#include "../Roads/Road.hpp"


void setup(){
	data = loadJSON("data.json");
	noCanvas();

	auto roads = data.roads;
	for (int i = 0; i < roads.length; ++i) {
		Roads::Road()


	}


	