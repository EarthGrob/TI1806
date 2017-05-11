//Collaborated with Bas Musters, 
//Co-created by Earth Grob, 4559746


#ifndef EXERCISES_H
#define EXERCISES_H

#include <list>
#include <set>
#include <map>
#include <queue>

////////////////// Exercise 1 ////////////////////////////////////
std::pair<float, float> Statistics(const std::list<float>& values)
{
	float sum = 0;
	for (float i : values) {
		sum += i;
	}
	float mean = sum / values.size();

	float var = 0;
	for (float i : values){
		var += (i - mean)*(i - mean);
	}

	float temp = var / values.size();
	float sd = sqrt(temp);


	return std::pair<float, float>(mean, sd);
}
//////////////////////////////////////////////////////////////////

////////////////// Exercise 2 ////////////////////////////////////
class TreeVisitor {
public:
	bool b = false;

	float visitTree(const Tree& tree, bool countOnlyEvenLevels) {
		float sum = 0.0;

		if (b == false) {
			b = countOnlyEvenLevels;
		}
		if (b) {
			for (auto it = tree.children.begin(); it != tree.children.end(); it++) {
				sum += visitTree(*it, !countOnlyEvenLevels);
			}
			if (countOnlyEvenLevels) {
				sum += tree.value;
			}
			return sum;
		}

		for (auto it = tree.children.begin(); it != tree.children.end(); it++) {
			sum += visitTree(*it, false);
		}
		sum += tree.value;
		return sum;
	}
};
//////////////////////////////////////////////////////////////////

////////////////// Exercise 3 ////////////////////////////////////
class Complex {
public:
	float r, i;

	Complex(float real, float imaginary) {
		r = real;
		i = imaginary;
	};

	Complex operator () (Complex& c1) {
		return c1;
	};
};
Complex operator + (Complex& c1, Complex& c2) {
	return Complex(c1.r + c2.r, c1.i + c2.i);
};

Complex operator - (Complex& c1, Complex& c2) {
	return Complex(c1.r - c2.r, c1.i - c2.i);
};

Complex operator * (Complex& c1, Complex& c2) {
	return Complex((c1.r*c2.r) - (c1.i*c2.i), (c1.r*c2.i) + (c2.r*c1.i));
};
//////////////////////////////////////////////////////////////////

////////////////// Exercise 4 ////////////////////////////////////
float WaterLevels(std::list<float> heights)
{
	
	int left = 0;
	int right = heights.size() - 1;
	float water = 0.0;
	float maxLeft = 0.0;
	float maxRight = 0.0;
	int i = 0;
	int size = heights.size();
	float* array = new float[size];

	//load all floats into the new array, so it is indexed
	for (float f : heights) {
		array[i] = f;
		i++;
	}
	

	while (left <= right) {											//as long as there exists an array
		if (array[left] <= array[right]) {							//find the highest position from the left
			if (array[left] >= maxLeft) maxLeft = array[left];		//if position on the left is the highest float, set new maxleft
			else water += maxLeft - array[left];					//else, calculate the 
			left++;													//move one position to the right
		}
		else {														//do the same thing from the right.
			if (array[right] >= maxRight) maxRight = array[right];
			else water += maxRight - array[right];
			right--;
		}
	}
	

	return water;
}


//////////////////////////////////////////////////////////////////

////////////////// Exercise 5 ////////////////////////////////////
typedef std::pair<int, int> location;

int breadthfirst(std::set<std::pair<location, location> > labyrinth, int size) {
	std::set<location> known;					//what locations are already visited?
	std::map<location, location> predecessors;  //from which location was this location found
	std::queue<location> q;						//a queue with the 'colored' locations/vertices
	location s = location(0, 0);				//the starting location
	auto notFound = labyrinth.end();

	q.push(s);			//we color s and
	known.insert(s);    //visit s already

	while (!q.empty()) {
		location current = q.front();	//
		q.pop();						//current = q.pop(); wasn't working

		int x = current.first;	//storing the x and
		int y = current.second;	// y of the current location

								//visit(current);
		if (x == size - 1 && y == size - 1) { // if the final location is reached
			break;							// stop searching
		}
		//end visit(current);

		//            up,do, le,ri
		int xDir[] = { 0, 0, -1, 1 };
		int yDir[] = { -1, 1, 0, 0 };
		for (int i = 0; i < 4; i++) { //going in all directions around current
			int newX = x + xDir[i];	//storing the x
			int newY = y + yDir[i]; // and y of the new location
									//looking if this new location is possible
			bool inBounderies = (newX >= 0 && newX < size && newY >= 0 && newY < size);
			location v = location(newX, newY);

			//looking if this location can be accessed by the previous location (is there a wall in between?)
			bool noWall = false;
			if (i % 2 == 0) { //going up or left
				noWall = labyrinth.find(std::pair<location, location>(v, current)) == notFound;
			}
			else { //going down or right
				noWall = labyrinth.find(std::pair<location, location>(current, v)) == notFound;
			}

			//if the location is possible, still unknown, and accessible from the previous(there is no wall in between)
			if (inBounderies && known.find(v) == known.end() && noWall) {
				q.push(v);					//then color it, to visit it later
				known.insert(v);			//make it known
				predecessors[v] = current;	//store its predecessor
			}
		}
	}

	//predecessors now contains the shortest path from the end to the start.
	location l = location(size - 1, size - 1);	//start at the end
	int distance = 1;							//set the distance to 1
	while (l.first != 0 || l.second != 0) { //while the beginning location is not reached
		l = predecessors[l];				//go back one location
		distance++;							//and add 1 to the distance
	}
	return distance;
};
int Labyrinth(std::set<std::pair<location, location> > labyrinth, int size)
{
	return breadthfirst(labyrinth, size);
};

//////////////////////////////////////////////////////////////////

#endif
