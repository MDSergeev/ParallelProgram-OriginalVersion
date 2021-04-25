#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include "PointTrial.h"

double f1(double x) {
	return exp(-0.5 * x) * sin(6 * x - 1.5);
}

double f2(double x) {
	return fabs(x) * sin(2 * M_PI * x - 0.5);
}

int main() {
	
	double x;
	
	std::cout << "x = ";
	std::cin >> x;

	std::vector<std::function<double(double)>> funcs;
	funcs.push_back(f1);
	funcs.push_back(f2);

	std::cout << "Calculate values: " << std::endl;
	for (std::function<double(double)> func : funcs) {
		std::cout << func(x) << std::endl;
	}
	
	
	return 0;
}