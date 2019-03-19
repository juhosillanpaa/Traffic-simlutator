#include "Human.hpp"

#ifdef DEBUG_MSG
#include <iostream>
#endif

namespace Humans {

	// Constants in hours
	#define WORKDAY_START_MIN 6
	#define WORKDAY_START_DELTA 4
	#define WORKDAY_LENGTH_MIN 6
	#define WORKDAY_LENGTH_DELTA 4
	#define HOBBY_START_DELTA 1
	#define HOBBY_LENGTH_MIN 1
	#define HOBBY_LENGTH_DELTA 2

	// Initialize static member
	std::mt19937 Human::rng(time(0)); // This is not a proper seed, do we want better?

	Intention create_intention(IntentionType type, Buildings::Building* building, tm* minStartTime, unsigned int maxStartDelay, unsigned int minLength, unsigned int maxLengthDelay) {

		time_t startTime;
		time_t endTime;

		std::uniform_int_distribution<> distributionStartHour(0, maxStartDelay - 1);
		std::uniform_int_distribution<> distributionEndHour(0, maxLengthDelay - 1);
		std::uniform_int_distribution<> distributionMinSec(0, 59);
		
		// Randomize start time
		minStartTime->tm_hour += distributionStartHour(Human::rng);
		minStartTime->tm_min += distributionMinSec(Human::rng);
		minStartTime->tm_sec += distributionMinSec(Human::rng);
		startTime = mktime(minStartTime);

		// Randomize end time
		minStartTime->tm_hour += minLength + distributionEndHour(Human::rng);
		minStartTime->tm_min += distributionMinSec(Human::rng);
		minStartTime->tm_sec += distributionMinSec(Human::rng);
		endTime = mktime(minStartTime);
		
		Intention newIntention = {
			type,
			building,
			startTime,
			endTime
		};

		return newIntention;
	}

	void Human::set_intentions() {

		// Decide when to work
		tm* timeInfo = localtime(&simulationTime);
		timeInfo->tm_hour = WORKDAY_START_MIN;
		intentions.push_back(create_intention(Work, &work, timeInfo, WORKDAY_START_DELTA, WORKDAY_LENGTH_MIN, WORKDAY_LENGTH_DELTA));


		// Decide when to go to hobby
		timeInfo = localtime(&(intentions.back().endTime));
		timeInfo->tm_hour += 1; // one hour after workday ended
		intentions.push_back(create_intention(Hobby, &hobby, timeInfo, HOBBY_START_DELTA, HOBBY_LENGTH_MIN, HOBBY_LENGTH_DELTA));

	}

	bool Human::is_on_the_road() {
		return currentBuilding->id != destinationBuilding->id;
	}

	time_t Human::next_departure() {
		if (intentions.begin() == intentions.end()) {
			return 0;
		}

		if (currentBuilding->id == home.id) {
			return intentions.begin()->startTime;
		} else {
			return intentions.begin()->endTime;
		}
	}

	void Human::leave_building(Engine::Engine* engine) {
		if (intentions.begin() == intentions.end()) {
			return;
		}
		
		if (currentBuilding->id == home.id) {
			destinationBuilding = intentions.front().building;
		} else {
			intentions.erase(intentions.begin()); // erase intention when leaving
			destinationBuilding = &home; // leave home for now
		}
		std::vector<Human*> passengers;
		passengers.push_back(this);

		#ifdef DEBUG_MSG
		std::cout << "Human moving from building " << currentBuilding->id << " to " << destinationBuilding->id << std::endl;
		#endif
		
		car.new_trip(engine, currentBuilding, destinationBuilding, passengers);

		
	}

	void Human::arrived_to_building() {
		#ifdef DEBUG_MSG
		std::cout << "Human arrived from building " << currentBuilding->id << " to " << destinationBuilding->id << std::endl;
		#endif

		currentBuilding = destinationBuilding;
	}

} // namespace Humans