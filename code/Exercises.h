#ifndef EXERCISES_H
#define EXERCISES_H

#include <list>
#include <set>

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
	standerdDeviation = sqrt(standerdDeviation/size); //standerdDeviation will become the squareroot of standerdDeviation
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
