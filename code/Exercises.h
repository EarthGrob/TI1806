#ifndef EXERCISES_H
#define EXERCISES_H

#include <list>
#include <set>

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
	Complex(float real, float imaginary){};

	float real, im;
};
//////////////////////////////////////////////////////////////////

////////////////// Exercise 4 ////////////////////////////////////
float WaterLevels(std::list<float> heights)
{
	return 0;
}
//////////////////////////////////////////////////////////////////

////////////////// Exercise 5 ////////////////////////////////////
typedef std::pair<int, int> location;

int Labyrinth(std::set<std::pair<location, location> > labyrinth, int size)
{
	return 0;
}
//////////////////////////////////////////////////////////////////

#endif
