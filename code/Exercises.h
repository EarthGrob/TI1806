#ifndef EXERCISES_H
#define EXERCISES_H

#include <list>
#include <set>
#include <queue>
#include <map>

////////////////// Exercise 1 ////////////////////////////////////
std::pair<float, float> Statistics(const std::list<float>& values)
{
	if (values.empty()) {
		return std::pair<float, float>(0.f, 0.f);
	}
	int size = values.size();		//storing the size in a local variable for faster acces
	float avg = 0.f;				//setting average and standerdDeviation to zero
	float standerdDeviation = 0.f;  //so that we can add values to them.

	for (float i: values) {			//foreach loop over the values of values
		avg += i;					//adding everything to avg so it becomes the total sum
	}
	avg = avg/size;					//dividing avg by the number of values so that it becomes the average of the values

	for (float i : values) {		//foreach over the values again
		float temp = (i-avg);		//storing the current value minus the average
		standerdDeviation += temp*temp; //adding the temporary value to standerdDeviation (it will become a sum of squares).
	}
	standerdDeviation = sqrt(standerdDeviation/size); //standerdDeviation will become the squareroot of standerdDeviation/size
	return std::pair<float, float>(avg, standerdDeviation); //returning the average first, then the standerdDeviation.
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
	float real, im;

	Complex(float rea, float imag){
		real = rea;
		im = imag;
	};

	Complex operator () (Complex& c1) {
		return c1;
	};
};
Complex operator + (Complex& c1, Complex& c2) {
	return Complex(c1.real + c2.real, c1.im + c2.im);
};

Complex operator - (Complex& c1, Complex& c2) {
	return Complex(c1.real - c2.real, c1.im - c2.im);
};

Complex operator * (Complex& c1, Complex& c2) {
	float r1 = c1.real;
	float i1 = c1.im;
	float r2 = c2.real;
	float i2 = c2.im;
	return Complex(r1*r2 - i1*i2, r1*i2 + r2*i1);
};
//////////////////////////////////////////////////////////////////

////////////////// Exercise 4 ////////////////////////////////////
int giveIndexOfHighest(std::list<float>& heights, int& from, int& to) {
	int indexOfHighest = -1;
	float highest = FLT_MIN;
	int count = 0;
	for (float f : heights) {
		if (count >= from && count <= to) {
			if (f >= highest) {
				highest = f;
				indexOfHighest = count;
			}
		}
		count++;
	}
	return indexOfHighest;
};
float WaterLevels(std::list<float> heights)
{
	if (heights.empty()) return 0;

	int size = heights.size();				//storing the size for faster access
	float* heightsArr = new float[size];	///////////////////////////////
											//
	int index = 0;							//	
	for (float f : heights) {				//	}copy the list into an array
		heightsArr[index] = f;				//
		index++;							//
	}										///////////////////////////////

	float waterTotal = 0.f;		//set the water total to zero
	int leftPtr = 0;			//set the left pointer to the first item of the array
	int rightPtr = size - 1;	// and the right pointer to the last
	float maxleft = 0.f;		//
	float maxright = 0.f;
	while (leftPtr <= rightPtr) {
		if (heightsArr[leftPtr] <= heightsArr[rightPtr]) {
			if (heightsArr[leftPtr] >= maxleft) {
				maxleft = heightsArr[leftPtr];
			} else {
				waterTotal += maxleft - heightsArr[leftPtr];
			}
			leftPtr++;
		} else {
			if (heightsArr[rightPtr] >= maxright) {
				maxright = heightsArr[rightPtr];
			} else {
				waterTotal += maxright - heightsArr[rightPtr];
			}
			rightPtr--;
		}
	}

	return waterTotal;
};
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
			} else { //going down or right
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
