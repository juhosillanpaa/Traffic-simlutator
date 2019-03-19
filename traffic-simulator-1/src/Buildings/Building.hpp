#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <vector>
#include "../Roads/Road.hpp"

namespace Buildings {

	class Building {
	public:
		Building(Roads::Road& homeRoad, float positionOnRoad, int id, int capacity) :
		homeRoad(homeRoad), positionOnRoad(positionOnRoad), id(id), capacity(capacity)
		{ }
		
		Roads::Road& homeRoad;
		const float positionOnRoad;
		const int id;
		const int capacity;
	};

} // namespace Buildings

#endif // include guard