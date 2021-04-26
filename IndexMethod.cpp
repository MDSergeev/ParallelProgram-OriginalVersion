#include "IndexMethod.h"
#include <stdexcept>

IndexMethod::IndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs) {
	if (leftBound < rightBound) {
		leftBound_ = leftBound;
		rightBound_ = rightBound;
	}
	else {
		throw std::invalid_argument::invalid_argument("Left bound is more than right bound");
	}

	if (eps > 0) {
		eps_ = eps;
	}
	else {
		throw std::invalid_argument::invalid_argument("eps <= 0");
	}

	if (!funcs.empty()) {
		funcs_ = funcs;
	}
	else {
		throw std::invalid_argument::invalid_argument("List of funcs is empty");
	}

	if (paramR > 1) {
		paramsR_.resize(funcs_.size());
		std::fill(paramsR_.begin(), paramsR_.end(), paramR);
	}
	else {
		throw std::invalid_argument::invalid_argument("Parameter of reliability is less than 1");
	}

	// Заносим граничные точки.
	trials_.insert(PointTrial(leftBound_, NULL, -1));
	trials_.insert(PointTrial(rightBound_, NULL, -1));

	// Задаём размеры массивов (по числу функций).
	fixedIndex_.resize(funcs_.size());
	lowerBounds_.resize(funcs_.size());
	paramsZ_.resize(funcs_.size());

	std::fill(lowerBounds_.begin(), lowerBounds_.end(), 1);

	maxIndex_ = -1;
}
