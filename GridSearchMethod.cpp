#include "GridSearchMethod.h"

GridSearchMethod::GridSearchMethod(double leftBound, double rightBound, double eps, const Funcs& funcs)
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
}

PointTrial GridSearchMethod::Run()
{
	PointTrial bestTrial = PointTrial(leftBound_, NULL, -1);
	double currX = leftBound_ + eps_;

	while (currX <= rightBound_)
	{
		PointTrial currTrial = newTrial(currX);
		if ((currTrial.getIndex() > bestTrial.getIndex())
			|| (currTrial.getIndex() == bestTrial.getIndex() && currTrial.getValue() < bestTrial.getValue())) {
			bestTrial = currTrial;
		}
		currX += eps_;
	}

	return bestTrial;
}

PointTrial GridSearchMethod::newTrial(double x)
{
	int index = -1;
	double value = NULL;
	for (std::size_t i = 0; i < funcs_.size(); ++i) {
		index = i;
		value = funcs_[i](x);
		if (value >= 0) {
			break;
		}
	}

	return PointTrial(x, value, index);
}

