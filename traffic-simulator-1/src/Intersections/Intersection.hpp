#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <string>
#include <list>
#include <algorithm>
#include <vector>
#include <ctime>
#include <utility>
#include "../Vehicles/Car.hpp"
#include "../Roads/Road.hpp"



namespace Intersections {


	class Intersection{
		/*
		intersections are made of 4 blocs/coordinate squares. its position is the position of a bottom left square.
		intersections have only plain red/green lighs, meaning there is no lights with arrows etc.

		MANUAL:
		Constructor: 1. vector of roads connected to it, 2. id to specify it, 3. vector of floats containing its position
		User must ask if it can use intersection using can_cross function 
		User must let intersection know when it has exited intersection using leaved-function
		Intersection starts working when function activate is used
		update_lights function checks if an update is needed and if it is then it updates the lights
		*/
		
	public:

		Intersection(std::vector<Roads::Road*> roads, int id, std::pair<int,int> pos) : roads(roads), id(id), position(pos) {}


		//function for car to communicate with  x = end of a current road, y = beginning of the next road

		bool can_cross(std::pair<int,int> x, std::pair<int,int> y);
		// helper function for can_cross, checks if its okay to turn left( no one is using the road car crosses)
		bool is_space(int var1);
		void change_lights();
		//function to tell intersection car no longer uses it, x = end of a road car used intersection from( same as in can_cross)

		void leaved(std::pair<int,int> v);	
		int helper(int var2, int var3);
		int helper2(std::pair<int,int> v1, std::pair<int,int> v2 );	
		void activate();
		void update_lights();
		std::vector<Roads::Road*> get_roads() { return roads; }

	private:
		
		int id;
		std::vector<int> lights;	

		std::pair<int,int> position;
		std::vector<int> users; 	//vector of cars currently using the intersections == cars allowed to cross it, doesnt include casrs behind red lights
		bool want_change = 0; 	// = 1 if someone wants to cuse lights that are red atm
		time_t light_timer;		//keeps track of time how long certain lights have been on

		std::vector<Roads::Road*> roads;



	};// class Intersection







} // namespace Intersections



#endif 