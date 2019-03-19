#include <math.h>
#include "Car.hpp"
#include "../Humans/Human.hpp"
#include "../Intersections/Intersection.hpp"
#include "../Engine/Engine.hpp"

#ifdef DEBUG_MSG
#include <iostream>
#endif

namespace Vehicles {
	#define TURNTIME 4

	void Car::new_trip(Engine::Engine* engine, Buildings::Building* origin, Buildings::Building* destination, std::vector<Humans::Human*> newPassengers) {
		passengers = newPassengers;
		currentDestination = destination;
		
		#ifdef DEBUG_MSG
			std::cout << "Car getting a new route" << std::endl;
		#endif

		route = engine->get_route(origin, destination);
		#ifdef DEBUG_MSG
			std::cout << "Car got a new route" << std::endl;
		#endif
		if (route.empty()) {
			arrived_to_destination();
		}
		
	}


	void Car::drive(Car* carInFront, unsigned int timeStep) {
		//check if there is route at all
		if (!get_current_road()) {
			#ifdef DEBUG_MSG
				std::cout << "Driving without a route!" << std::endl;
			#endif
			return;
		}


		// check if we are at destination
		if (get_current_road() == route.back() && currentDestination->positionOnRoad <= positionOnRoad) {
			arrived_to_destination();
			return;
		}


		//check if car is im intersection and if its okay to use intersection
		if (get_current_road()->get_length() <= positionOnRoad) {	
			positionOnRoad = get_current_road()->get_length();
			currentSpeed = 0;
			//car must be already turning
			if (turning){
				//check if it has spent nough time in turning
				if ( clock()-timer >= TURNTIME) {
					turning = 0;
					get_current_road()->get_end_intersection()->leaved(get_current_road()->get_endpoint());//let intersection know that we have exited it
					positionOnRoad = 0;
					currentSpeed = get_next_road()->get_speedlimit();
					route.erase(route.begin());
					#ifdef DEBUG_MSG
						if (route.empty()) {
							std::cout << "Arrived at the end of route but didn't arrive at destination" << std::endl;
						}
					#endif
				}
			} // end if turning

			//or it has just arrvied and wants to know if it can move
			else if (get_current_road()->get_end_intersection()->can_cross(get_current_road()->get_endpoint(), get_next_road()->get_startpoint() )) {
				turning = 1;
				timer = clock();
			}
			return;
		}


		//slow down before intersection, reduce currentspeed and move car in the end of this function
		if (positionOnRoad*(timeStep*currentSpeed*4) >= get_current_road()->get_length()) { // stop at the end of the road, before intersection,
			float distance = get_current_road()->get_length() - positionOnRoad;
			float t_left = distance/currentSpeed;
			float acc = currentSpeed/t_left;
			currentSpeed = currentSpeed - acc*timeStep;
		}


		// If there's a car in front, verify that we won't crash
		if (carInFront && carInFront->get_speed() < currentSpeed) { 
			// Calculate the time to crash if both cars keep the same speed
			float distance = carInFront->get_position() - positionOnRoad - carInFront->get_length();
			float timeToCrash = distance / (currentSpeed - carInFront->get_speed());

			#ifdef DEBUG_MSG
				if (timeToCrash < 0) {
					std::cout << "Uh oh. timeToCrash is negative somehow." << std::endl;
				}
			#endif

			if ((currentSpeed - carInFront->get_speed()) / deceleration < timeToCrash + timeStep + 2) {
				this->decelerate(timeStep, 1); // Hard braking
			} else if ((currentSpeed - carInFront->get_speed() / 2) / deceleration < timeToCrash + timeStep + 4) {
				this->decelerate(timeStep, 0.5f); // Moderate braking
			}
		} else {
			this->accelerate(timeStep);
		}


		//check if driving too fast
		if (currentSpeed > get_current_road()->get_speedlimit()) {
			currentSpeed = get_current_road()->get_speedlimit();
		}

		// move forward
		positionOnRoad += timeStep * currentSpeed;
	}




	Roads::Road* Car::get_next_road() {
		
		if (route.empty()) { 
			return NULL;
		}
		if (route.size() >= 2){
			return route.at(1);
		}
		return NULL;
	}

	Roads::Road* Car::get_current_road() {
		if (route.empty()) {
			return NULL;
		}
		return route.at(0);
	}

	void Car::arrived_to_destination() {
		route = {};
		currentDestination = NULL;

		for (auto passenger : passengers) {
			passenger->arrived_to_building();
		}
	}

}	//namescpace Car