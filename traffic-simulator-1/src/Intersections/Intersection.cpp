#include "Intersection.hpp"
#include <algorithm>



namespace Intersections {
	//time between changes in seconds
	#define INTERVAL 30

	void Intersection::update_lights() {
		if (want_change ) {
			if (clock() - light_timer > INTERVAL){
				Intersection::change_lights();
			}
		}
	}

	void Intersection::activate() {
		
		lights.push_back(1);
		lights.push_back(0);
		lights.push_back(1);
		lights.push_back(0);



		light_timer = clock();
		return;

	}

	void Intersection::leaved(std::pair<int,int> vect3) {

		int var = Intersection::helper(vect3.first, position.first );
		std::vector<int>::iterator pos = std::find(users.begin(), users.end(), var);
		if (pos != users.end()){
			users.erase(pos);
		}
	}

	void Intersection::change_lights() {
		want_change = 0;
		light_timer = clock();
		
		for (int i= 0; i < 4; ++i) {
			if (lights.at(i)  == 0) {
				lights.at(i) = 1;
			}
			else { lights.at(i) = 0; }
		}
	}

	bool Intersection::is_space(int var3) {
	int counter = (var3 + 2) % 4;		// counter = direction of car coming from opposite direction
		


		if (std::find(users.begin(), users.end(), counter) != users.end() ) {
			return false;
		}
		
		return true;
	}

	bool Intersection::can_cross(std::pair<int,int> vec1, std::pair<int,int> vec2) {
		//vect1 is end of a road car is currently on, vect 2 is start of a road car is travelling next

		int from = Intersection::helper(vec1.first, position.first);		//helper for determinating if lights are green or red, from = direction car is approaching
		

		if ( lights.at(from) == 0) {	//lights are red so append request for queue and return 0, car cannot cross
			want_change = 1;			//adding wish for green lights for this direction
			return 0;
		}

		int turn = Intersection::helper2(vec1, vec2);		//helper for determinating the turn, turn = left,right,straight
		users.push_back(from);											//adding car a user of intersection so others can check if the can move
		if (turn != 2) {			
			// lights are green and no one should be in way (car is turning right or going staright) so return 1
			return 1;
		}

		if ( Intersection::is_space(from) ) {
			//no one is using the opposite line so car is allowed to turn left
			return 1;
		}

		return 0;
	}
	int Intersection::helper2(std::pair<int,int> ve1, std::pair<int,int> ve2 ) {
		// return 1 if car wants to go straight, 0 if turn right and 2 if turn left
		if ( (ve1.first == ve2.first ) || (ve1.second == ve2.second ) ) {
			return 1;
		}
		else if ((ve1.first + 1 == ve2.first ) || (ve1.first -1 == ve2.first )) {
			return 0;
		}
		return 2;
	}
	int Intersection::helper(int x1, int y1) {
		//return 0 if car approachs from top, 1 if from right, 2 if bottom and 3 if left
		int temp = y1 - x1;
		if (temp == 0 ){ 
			return 0;
		}

		else if ( temp == -2 ) {
			return 1;
		}
		else if ( temp == -1 ){
			return 2;
		}
		return 3;
	}

} // namespace Intersections




	


