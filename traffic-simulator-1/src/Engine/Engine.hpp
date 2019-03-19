#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <vector>
#include <string>

#include "../Buildings/Building.hpp"
#include "../Intersections/Intersection.hpp"
#include "../Humans/Human.hpp"
#include "../Roads/Road.hpp"

namespace Engine {

	struct Route {
		Buildings::Building* origin;
		Buildings::Building* destination;
		std::vector<Roads::Road*> roads;
		Route() { origin = NULL; destination = NULL; roads = {}; }
	};

	class Engine {
	public:
		Engine(); // Constructor for sample simulation
		Engine(std::string filename); // Constructor for simulation from a file

		Engine(const Engine&) = delete; //No copy constructor.
		Engine& operator=(Engine const&) = delete; // No assignment constructor.

		std::vector<Roads::Road*> get_route(Buildings::Building* origin, Buildings::Building* destination);
		void construct_sample_city();
		void simulate();

	private:
		
		Route find_new_route(Buildings::Building* origin, Buildings::Building* destination);
		
		void notify_persons_to_leave();

		std::vector<Buildings::Building> buildings;
		std::vector<Humans::Human> persons;
		std::vector<Humans::Human*> personPointers;
		std::vector<Roads::Road> roads;
		std::vector<Route> routes;

		time_t dayStart;
		time_t simulationTime;
	};

} // namespace Engine	

#endif //include guard