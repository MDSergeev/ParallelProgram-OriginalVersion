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
	maxValuesDifference_.resize(funcs_.size());
	paramsZ_.resize(funcs_.size());

	std::fill(maxValuesDifference_.begin(), maxValuesDifference_.end(), 1);

	maxIndex_ = -1;
}

PointTrial IndexMethod::Run()
{
	// Первое испытание проводится в серединной точке.
	PointTrial bestTrial = newTrial((rightBound_ + leftBound_) / 2);
	trials_.insert(bestTrial);
	maxIndex_ = bestTrial.getIndex();

	bool stop = false;
	while (!stop) {
		calculateFixedIndex();
		calculateMaxValuesDifference();
		calculateZ(bestTrial);
		
		std::vector<PointTrial> currInterval = calculateMaxR();
		if (fabs(currInterval[1].getX() - currInterval[0].getX()) < eps_) {
			stop = true;
		}
		else {
			// Вычисляем новое испытание.
			PointTrial currTrial = newTrial(currInterval);
			// Добавляем новое испытание во множество испытаний.
			trials_.insert(currTrial);
			if ((currTrial.getIndex() > maxIndex_)
				|| (currTrial.getIndex() == maxIndex_ && currTrial.getValue() < bestTrial.getValue())) {
				bestTrial = currTrial;
				maxIndex_ = currTrial.getIndex();
			}
		}
	}
	return bestTrial;
}

void IndexMethod::calculateFixedIndex()
{
	// Очистка множества I.
	for (std::vector<std::set<PointTrial>>::iterator it = fixedIndex_.begin(); it != fixedIndex_.end(); ++it) {
		(*it).clear();
	}

	// Заполнение множества I для всех внутренних точек.
	for (std::set<PointTrial>::iterator it = ++trials_.cbegin(); it != --trials_.cend(); ++it) {
		fixedIndex_[(*it).getIndex()].insert(*it);
	}
}

void IndexMethod::calculateMaxValuesDifference()
{
	// Внешний цикл - по каждому номеру индекса.
	for (unsigned int i = 0; i < fixedIndex_.size(); ++i) {
		if (fixedIndex_[i].size() < 2) {
			maxValuesDifference_[i] = 1;
		}
		else {
			double maxM = -HUGE_VAL;
			double tempM = 0;
			for (std::set<PointTrial>::iterator it = ++fixedIndex_[i].cbegin(); it != fixedIndex_[i].cend(); ++it) {
				// Вспомогательный итератор.
				std::set<PointTrial>::iterator it2 = it;
				--it2;
				// Вычисляем |z(i) - z(i-1)| / (x(i) - x(i-1)).
				tempM = fabs((*it).getValue() - (*it2).getValue()) / ((*it).getX() - (*it2).getX());
				if (tempM > maxM) {
					maxM = tempM;
				}
			}
			if (maxM > 0) {
				maxValuesDifference_[i] = maxM;
			}
			else {
				maxValuesDifference_[i] = 1;
			}
		}
	}
}

void IndexMethod::calculateZ(const PointTrial& bestTrial)
{
	for (unsigned int i = 0; i < paramsZ_.size(); ++i) {
		if (i == maxIndex_) {
			bestTrial.getValue();
		}
		else {
			paramsZ_[i] = 0;
		}
	}
}

std::vector<PointTrial> IndexMethod::calculateMaxR()
{
	std::vector<PointTrial> result(2);
	double maxR = -HUGE_VAL;

	for (std::set<PointTrial>::iterator it = ++trials_.cbegin(); it != trials_.cend(); ++it) {
		// Текущая точка x(i).
		PointTrial pointCurr = *it;
		std::set<PointTrial>::iterator it2 = it;
		// Предыдущая точка x(i-1).
		PointTrial pointPrev = *(--it2);
		

		double currR = 0;
		double delta = pointCurr.getX() - pointPrev.getX();

		if (pointPrev.getIndex() == pointCurr.getIndex()) {
			int index = pointPrev.getIndex();
			currR = delta
				+ pow(pointCurr.getValue() - pointPrev.getValue(), 2) / (delta * pow(paramsR_[index], 2) * pow(maxValuesDifference_[index], 2)) 
				- 2 * (pointCurr.getValue() + pointPrev.getValue() - 2 * paramsZ_[index]) / (paramsR_[index] * maxValuesDifference_[index]);
		}
		else if (pointPrev.getIndex() < pointCurr.getIndex()) {
			int index = pointCurr.getIndex();
			currR = 2 * delta - 4 * (pointCurr.getValue() - paramsZ_[index]) / (paramsR_[index] * maxValuesDifference_[index]);
		}
		else {
			int index = pointPrev.getIndex();
			currR = 2 * delta - 4 * (pointPrev.getValue() - paramsZ_[index]) / (paramsR_[index] * maxValuesDifference_[index]);
		}

		if (currR > maxR) {
			maxR = currR;
			result[0] = pointPrev;
			result[1] = pointCurr;
		}
	}

	return result;
}

PointTrial IndexMethod::newTrial(double x)
{
	int index = 0;
	double value = funcs_[0](x);
	for (unsigned int i = 1; i < funcs_.size(); ++i) {
		index = i;
		value = funcs_[i](x);
		if (value > 0) {
			break;
		}
	}

	return PointTrial(x, value, index);
}

PointTrial IndexMethod::newTrial(const std::vector<PointTrial>& interval)
{
	if (interval[0].getIndex() != interval[1].getIndex()) {
		double newX = (interval[0].getX() + interval[1].getX()) / 2;
		return newTrial(newX);
	}
	else {
		int index = interval[0].getIndex();
		double newX = (interval[0].getX() + interval[1].getX()) / 2
			- (interval[0].getValue() - interval[1].getValue()) / (2 * paramsR_[index] * maxValuesDifference_[index]);
		return newTrial(newX);
	}
}



