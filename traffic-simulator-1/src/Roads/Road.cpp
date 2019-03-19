#include <math.h>

#include "Road.hpp"

namespace Roads {

	int distance_between(std::pair<int,int> first, std::pair<int,int> second) {
		int x_square = pow(first.first - second.first, 2);
		int y_square = pow(first.second - second.second, 2);
		return pow(x_square + y_square, 0.5f);
	}

	std::string Road::get_name() {
		return name;
	}

	std::pair<int,int> Road::get_startpoint() {
		return startpoint;
	}

	std::pair<int,int> Road::get_endpoint() {
		return endpoint;
	}

	int Road::get_speedlimit() {
		return speedlimit;
	}

	float Road::get_length() {
		float actualCurvature = curvature;

		if (actualCurvature < 1) {
			actualCurvature = 1;
		}

		return distance_between(startpoint, endpoint) * actualCurvature;
	}




}	// namespace Roads