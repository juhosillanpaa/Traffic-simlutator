#ifndef CAR_HPP
#define CAR_HPP

#include <vector>
#include "../Buildings/Building.hpp"
#include "../Roads/Road.hpp"
#include <ctime>

namespace Humans {
	class Human; // Forward declaration to solve cross referencing
} // namespace Humans

namespace Engine {
	class Engine;
}

namespace Vehicles {

	class Car {

		public:
			Car(unsigned int capacity_) :
			capacity(capacity_), length(1), acceleration(0.6), deceleration(0.8) {}

		void new_trip(Engine::Engine* engine, Buildings::Building* origin, Buildings::Building* destination, std::vector<Humans::Human*> newPassengers);
		//following function not working
		/*
		void start_driving(Roads::Road& road_, std::vector<int> position_);	// place car on a spesific place on certain road
		*/

		// Engine tells the car that it's its turn to drive and gives a pointer to the next car
		// or a null pointer if this car is the first car on the road
		void drive(Car* carInFront, unsigned int timeStep = 1);

		Roads::Road* get_current_road();
		Roads::Road* get_next_road();
		float get_speed() { return currentSpeed; }
		float get_length() { return length; }
		float get_position() { return positionOnRoad; }

		private:
			// For tasks when arriving to the destination:
			// *Tell each passenger that they have arrived to the destination.
			void arrived_to_destination();
			void accelerate(unsigned int timeStep, float intensity = 1) { currentSpeed += timeStep * intensity * acceleration; }
			void decelerate(unsigned int timeStep, float intensity = 1) { currentSpeed -= timeStep * intensity * deceleration; }

			Buildings::Building* currentDestination;
			const unsigned int capacity;
			float currentSpeed;
			const float length;
			const float acceleration;
			const float deceleration;
			time_t timer;	//timer for time spend in intersection
			bool turning;


			float positionOnRoad;
			std::vector<Humans::Human*> passengers;
			std::vector<Roads::Road*> route;
			//Roads::Road& currentRoad;

	};
} // namespace Vehicles




#endif //include guard