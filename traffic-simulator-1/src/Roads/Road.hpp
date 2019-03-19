#ifndef ROAD_HPP
#define ROAD_HPP

#include <vector>
#include <string>
#include <utility>

namespace Intersections {
	class Intersection;
}

namespace Roads {

	class Road {

	public:
		Road(std::pair<int,int> startpoint, std::pair<int,int> endpoint, float curvature, std::string name, int speedlimit, Intersections::Intersection* endIntersection = NULL)
		: startpoint(startpoint), endpoint(endpoint), curvature(curvature), name(name), speedlimit(speedlimit) {}

		std::string get_name();s
		std::pair<int,int> get_startpoint();
		std::pair<int,int> get_endpoint();
		int get_speedlimit();
		float get_length();
		void set_end_intersection(Intersections::Intersection* intersection) { endIntersection = intersection; }
		Intersections::Intersection* get_end_intersection() { return endIntersection; }

	private:
		const std::pair<int,int> startpoint;		// coordinates of the start of the road 
		const std::pair<int,int> endpoint;			// coordinates of the end of the road
		const float curvature;
		const std::string name;						// name of the road
		Intersections::Intersection* endIntersection;
		int speedlimit;

	};	// Class Road

} 	//namespace Roads

#endif //include guard