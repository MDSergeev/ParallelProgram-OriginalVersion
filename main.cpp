#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <iomanip>
#include "OriginalIndexMethod.h"
#include "GridSearchMethod.h"

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
	
	OriginalIndexMethod im(a, b, eps, r, funcs);
	PointTrial imBestTrial = im.Run();
	std::cout << "BEST TRIAL INDEX METHOD" << std::endl;
	std::cout << std:: fixed << std::setprecision(5)
		<< "x = " << imBestTrial.getX() << " | "
		<< "value = " << imBestTrial.getValue() << " | "
		<< "index = " << imBestTrial.getIndex() << " |" << std:: endl;
	
	GridSearchMethod gsm(a, b, eps, funcs);
	PointTrial gsmBestTrial = gsm.Run();
	std::cout << "BEST TRIAL GRID SEARCH METHOD" << std::endl;
	std::cout << std:: fixed << std::setprecision(5)
		<< "x = " << gsmBestTrial.getX() << " | "
		<< "value = " << gsmBestTrial.getValue() << " | "
		<< "index = " << gsmBestTrial.getIndex() << " |" << std::endl;
	
	return 0;
}