#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <ctime>
#include "OriginalIndexMethod.h"
#include "GridSearchMethod.h"
#include "ParallelIndexMethod.h"

double f0(double x) {
	return exp(-0.5 * x) * sin(6 * x - 1.5);
}

double f1(double x) {
	return fabs(x) * sin(2 * M_PI * x - 0.5);
}

double f2(double x) {
	return sin(2 * M_PI * x) * sqrt(fabs(x));
}

double f3(double x) {
	return (sqrt(fabs(x)) + 2 * cos(M_PI * x)) / (2 + sin(2 * M_PI * x));
}

double fi(double x) {
	return cos(18 * x - 3) * sin(10 * x - 7) + 1.5;
}

int main() {
	double a, b;
	double eps;
	double r;
	int N;
	
	std::cout << "a = ";
	std::cin >> a;

	std::cout << "b = ";
	std::cin >> b;

	std::cout << "eps = ";
	std::cin >> eps;

	std::cout << "Param R = ";
	std::cin >> r;

	std::cout << "Count Threads = ";
	std::cin >> N;

	Funcs funcs{f0, f1, f2, f3, fi};
	/*
	OriginalIndexMethod im(a, b, eps, r, funcs);
	unsigned int start_time = clock();
	PointTrial imBestTrial = im.Run();
	unsigned int end_time = clock();
	std::cout << "BEST TRIAL ORIGINAL INDEX METHOD" << std::endl;
	std::cout << std:: fixed << std::setprecision(5)
		<< "x = " << imBestTrial.x() << " | "
		<< "value = " << imBestTrial.value() << " | "
		<< "index = " << imBestTrial.index() << " |" << std:: endl;
	std::cout << "TIME = " << end_time - start_time << "ms" << std::endl;
	*/
	ParallelIndexMethod pim(a, b, eps, r, funcs, N);
	unsigned int start_time = clock();
	PointTrial pimBestTrial = pim.Run();
	unsigned int end_time = clock();
	std::cout << "BEST TRIAL PARALLEL INDEX METHOD" << std::endl;
	std::cout << std::fixed << std::setprecision(5)
		<< "x = " << pimBestTrial.x() << " | "
		<< "value = " << pimBestTrial.value() << " | "
		<< "index = " << pimBestTrial.index() << " |" << std::endl;
	std::cout << "TIME = " << end_time - start_time << "ms" << std::endl;

	GridSearchMethod gsm(a, b, eps, funcs);
	PointTrial gsmBestTrial = gsm.Run();
	std::cout << "BEST TRIAL GRID SEARCH METHOD" << std::endl;
	std::cout << std::fixed << std::setprecision(5)
		<< "x = " << gsmBestTrial.x() << " | "
		<< "value = " << gsmBestTrial.value() << " | "
		<< "index = " << gsmBestTrial.index() << " |" << std::endl;

	return 0;
}