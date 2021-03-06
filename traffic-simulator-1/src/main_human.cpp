#include <vector>
#include "Buildings/Building.hpp"
#include "Humans/Human.hpp"
#include "Vehicles/Car.hpp"
#include "Intersections/Intersection.hpp"
#include "Roads/Road.hpp"
#include <iostream>
#include <utility>

int main () {

	time_t simulationTime;
	time_t dayStart = time(0);
	dayStart -= (dayStart % (24 * 60 * 60)); // set dayStart to midnight
	simulationTime = dayStart;

	std::pair<int,int> vect4(2,0);
	std::pair<int,int> vect5(2,0);
	std::pair<int,int> vect6(2,2);

	std::string roadname = "Mannerheimintie";
	std::cout << "Construct road" << std::endl;
	Roads::Road road1(vect4, vect5, 2, roadname, 80);

	std::cout << "Construct car" << std::endl;
	Vehicles::Car car1(4);
	Vehicles::Car car2(2);

	std::vector<Roads::Road*> roads;
	roads.push_back(&road1);
	std::cout << "Construct intersection" << std::endl;
	Intersections::Intersection cross(roads, 11, vect6);

	std::cout << "Construct building" << std::endl;
	Buildings::Building home(road1, road1.get_length() / 4, 1, 50);
	Buildings::Building work(road1, road1.get_length() / 3, 2, 50);
	Buildings::Building hobby(road1, road1.get_length() / 2, 3, 50);

	std::cout << "Construct human" << std::endl;
	Humans::Human person1(simulationTime, home, work, hobby);
	Humans::Human person2(simulationTime, home, work, hobby);
	std::vector<Humans::Human*> persons;
	persons.push_back(&person1);
	persons.push_back(&person2);

	

	std::pair<int,int> vect7(3,0);
	std::pair<int,int> vect8(3,1);
	std::string n1 = "ekatie";

	std::pair<int,int> vect9(3,4);
	std::pair<int,int> vect10(3,5);
	std::string n2 = "tokatie";
	/* 5. argument missing
	Roads::Road road2(vect7, vect8, n1, 80);
	Roads::Road road3(vect9, vect10, n2, 80);
	
	*/




	std::cout << "Starting simulation" << std::endl;
	// Simulate a day second by second
	for (; simulationTime < dayStart + 24 * 60 * 60; simulationTime++) {

		// This loop checks if person should leave a building to move to next location
		for (auto person : persons) {
			if (person->is_on_the_road()) {
				continue;
			}
			time_t nextDeparture = person->next_departure();
			if (!nextDeparture) {
				// This person is no longer going anywhere today
				continue;
			}
			else if (nextDeparture <= simulationTime) {
				person->leave_building();
			}
		}
	}

}