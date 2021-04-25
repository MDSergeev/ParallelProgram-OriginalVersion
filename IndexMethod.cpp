#include "IndexMethod.h"
#include <stdexcept>

IndexMethod::IndexMethod(double leftBound, double rightBound, double eps, const Funcs& funcs) {
	if (leftBound < rightBound) {
		leftBound_ = leftBound;
		rightBound_ = rightBound;
	}
	else {
		throw std::invalid_argument::invalid_argument("Left bound more than right bound");
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
}