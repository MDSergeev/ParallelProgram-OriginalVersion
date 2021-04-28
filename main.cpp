#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include "IndexMethod.h"

double f1(double x) {
	return exp(-0.5 * x) * sin(6 * x - 1.5);
}

double f2(double x) {
	return fabs(x) * sin(2 * M_PI * x - 0.5);
}

double fi(double x) {
	return cos(18 * x - 3) * sin(10 * x - 7) + 1.5;
}

int main() {
	double a, b;
	double eps;
	double r;
	
	std::cout << "a = ";
	std::cin >> a;

	std::cout << "b = ";
	std::cin >> b;

	std::cout << "eps = ";
	std::cin >> eps;

	std::cout << "Param R = ";
	std::cin >> r;

	std::vector<std::function<double(double)>> funcs;
	funcs.push_back(f1);
	funcs.push_back(f2);
	funcs.push_back(fi);
	
	IndexMethod im(a, b, eps, r, funcs);
	PointTrial bestTrial = im.Run();
	std::cout << "BEST TRIAL" << std::endl;
	std::cout << "x = " << bestTrial.getX() << std::endl
		<< "value = " << bestTrial.getValue() << std::endl
		<< "index = " << bestTrial.getIndex();
		

	return 0;
}