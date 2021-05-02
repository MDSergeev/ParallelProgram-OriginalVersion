#include "IndexMethod.h"

IndexMethod::IndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs)
{
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
		paramR_ = paramR;
	}
	else {
		throw std::invalid_argument::invalid_argument("Parameter of reliability is less than 1");
	}
}
