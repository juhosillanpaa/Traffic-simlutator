#include <iostream>
#include "Engine.hpp"

namespace Engine {

	float calculateRouteTime(std::vector<Roads::Road*> route, float originBuildingPosition, float destinationBuildingPostition) {
		float time = 0;
		for (auto road = route.begin(); road != route.end(); road++) {
			float length = (*road)->get_length();
			if (road == route.begin()) {
				length -= originBuildingPosition;
			}
			if (road == route.end() - 1) {
				length -= (*road)->get_length() - destinationBuildingPostition;
			}
			time += (*road)->get_speedlimit() * length;
		}

		return time;
	}

	#ifdef DEBUG_MSG
	void print_route(std::vector<Roads::Road*> roads) {
		if (roads.empty()) return;

		for (auto road : roads) {
			std::cout << "    " << road->get_name() << std::endl;
		}
	}
	#endif


	Engine::Engine() {
		dayStart = time(0);
		dayStart -= (dayStart % (24 * 60 * 60)); // set dayStart to midnight
		simulationTime = dayStart;

		construct_sample_city();
		simulate();
	}

	Engine::Engine(std::string filename) {
		//TODO
	}

	std::vector<Roads::Road*> Engine::get_route(Buildings::Building* origin, Buildings::Building* destination) {
		// Return ready route if it already exists
		for (auto route : routes) {
			if (route.origin != origin) continue;
			if (route.destination != destination) continue;
			return route.roads;
		}

		Route newRoute = find_new_route(origin, destination);
		if (newRoute.origin == NULL) {
			std::cout << "Didn't find a route from building " << origin->id << " to building " << destination->id << std::endl;
			return { };
		}
		routes.push_back(newRoute);
		#ifdef DEBUG_MSG
			std::cout << "Found a new route from building " << origin->id << " to building " << destination->id << ":" << std::endl;
			print_route(newRoute.roads);
		#endif
		return newRoute.roads;
	}

	void Engine::construct_sample_city() {
		std::pair<int,int> vect4(2,0);
		std::pair<int,int> vect5(2,0);
		std::pair<int,int> vect6(2,2);

		std::string roadname = "Mannerheimintie";
		std::cout << "Construct road" << std::endl;
		Roads::Road road1(vect4, vect5, 2, roadname, 80);
		roads.push_back(road1);

		std::cout << "Construct intersection" << std::endl;
		//Intersections::Intersection cross(&roads, 11, vect6);

		std::cout << "Construct building" << std::endl;
		Buildings::Building home(roads.at(0), roads.at(0).get_length() / 4, 1, 50);
		Buildings::Building work(roads.at(0), roads.at(0).get_length() / 3, 2, 50);
		Buildings::Building hobby(roads.at(0), roads.at(0).get_length() / 2, 3, 50);
		buildings.push_back(home);
		buildings.push_back(work);
		buildings.push_back(hobby);

		std::cout << "Construct human" << std::endl;
		for (int i = 0; i < 1; i++) {
			persons.push_back(Humans::Human(simulationTime, buildings.at(0), buildings.at(1), buildings.at(2)));
			persons.back().set_intentions();
			personPointers.push_back(&persons.back());
		}
	}

	Route Engine::find_new_route(Buildings::Building* origin, Buildings::Building* destination) {
		std::vector<std::vector<Roads::Road*>> incompleteRoutes;
		std::vector<std::vector<Roads::Road*>> completeRoutes;
		std::vector<Roads::Road*> newRoute;
		newRoute.push_back(&(origin->homeRoad));
		incompleteRoutes.push_back(newRoute);

		// Find all possible routes
		while (!incompleteRoutes.empty()) {
			
			for (auto route = incompleteRoutes.begin(); route != incompleteRoutes.end(); route++) {
				if (route->back() == &(destination->homeRoad)) {
					// This route is complete
					completeRoutes.push_back(*route);
					incompleteRoutes.erase(route);
					route--;
					continue;
				}
				
				if (route->back()->get_end_intersection() == NULL || route->back()->get_end_intersection()->get_roads().empty()) {
					// This road leads to nowhere
					incompleteRoutes.erase(route);
					route--;
					continue;
				}

				// Create new possible routes from each route in the end intersection
				std::vector<Roads::Road*> newRoads = route->back()->get_end_intersection()->get_roads();
				for (auto newRoad : newRoads) {
					// if this road is not already on the route
					if (find(route->begin(), route->end(), newRoad) != route->end()) {
						std::vector<Roads::Road*> newRoute = *route;
						newRoute.push_back(newRoad);
						incompleteRoutes.push_back(newRoute);
					}
				}

				// Erase this route as it's not a possible route
				incompleteRoutes.erase(route);
				route--;
			}
		}

		// Find the fastest route
		if (completeRoutes.empty()) {
			return Route();
		}
		newRoute = completeRoutes.front();
		float fastestRouteTime = calculateRouteTime(newRoute, origin->positionOnRoad, destination->positionOnRoad);
		for (auto route = completeRoutes.begin(); route != completeRoutes.end(); route++) {
			if (calculateRouteTime(*route, origin->positionOnRoad, destination->positionOnRoad) < fastestRouteTime) {
				newRoute = *route;
			}
		}

		struct Route fastestRoute;
		fastestRoute.origin = origin;
		fastestRoute.destination = destination;
		fastestRoute.roads = newRoute;

		return fastestRoute;
	}

	void Engine::simulate() {
		// Simulate a day second by second
		for (; simulationTime < dayStart + 24 * 60 * 60; simulationTime++) {
			notify_persons_to_leave();
			// TODO: add person on road
			// TODO: advance all cars on each road a step
		}
	}

	void Engine::notify_persons_to_leave() {
		// This loop checks if person should leave a building to move to next location
		for (auto person : personPointers) {
			if (person->is_on_the_road()) {
				continue;
			}
			time_t nextDeparture = person->next_departure();
			if (!nextDeparture) {
				// This person is no longer going anywhere today
				continue;
			}
			else if (nextDeparture <= simulationTime) {
				person->leave_building(this);
			}
		}
	}
}