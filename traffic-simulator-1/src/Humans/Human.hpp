#ifndef HUMAN_HPP
#define HUMAN_HPP

#include <random>
#include <vector>
#include <ctime>
#include "../Buildings/Building.hpp"
#include "../Vehicles/Car.hpp"

namespace Engine {
	class Engine;
}

namespace Humans {

	enum IntentionType {
		Work,
		Hobby
	};

	struct Intention {
		IntentionType type;
		Buildings::Building* building;
		time_t startTime;
		time_t endTime;
	};

	class Human {
	public:
		Human(const time_t &simulationTime, Buildings::Building& home, Buildings::Building& work, Buildings::Building& hobby) :
		simulationTime(simulationTime), currentBuilding(&home), destinationBuilding(&home), home(home), work(work), hobby(hobby), car(Vehicles::Car(1))
		{ }

		void set_intentions();

		// Returns if the person is currently on the road
		bool is_on_the_road();
		// Use this to find out when the human is planning to leave the current building
		// Returns zero if there's no more intentions for today
		time_t next_departure();
		// Tell person that it's time to move to the next building
		void leave_building(Engine::Engine* engine);
		// Tell the person that they arrived where they were traveling
		void arrived_to_building();

		static std::mt19937 rng;

	private:
		const time_t& simulationTime;
		Buildings::Building* currentBuilding;
		Buildings::Building* destinationBuilding;
		Buildings::Building& home;
		Buildings::Building& work;
		Buildings::Building& hobby;

		Vehicles::Car car;

		std::vector<Intention> intentions;
	};

} // namespace Humans

#endif // include guard