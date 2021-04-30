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
		paramR_ = paramR;
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
	for (std::set<PointTrial>::const_iterator it = ++trials_.cbegin(); it != --trials_.cend(); ++it) {
		fixedIndex_[(*it).getIndex()].insert(*it);
	}
}

void IndexMethod::calculateMaxValuesDifference()
{
	// Внешний цикл - по каждому номеру индекса.
	for (std::size_t i = 0; i < fixedIndex_.size(); ++i) {
		if (fixedIndex_[i].size() < 2) {
			maxValuesDifference_[i] = 1;
		}
		else {
			double maxM = -HUGE_VAL;

			// Итератор на предыдущий элемент множества.
			std::set<PointTrial>::const_iterator itPrev = fixedIndex_[i].cbegin();
			// Итератор на текущий элемент множества.
			std::set<PointTrial>::const_iterator itCurr = ++fixedIndex_[i].cbegin();
			for (; itPrev != --fixedIndex_[i].cend() && itCurr != fixedIndex_[i].cend(); ++itPrev, ++itCurr) {
				// Вычисляем |z(i) - z(i-1)| / (x(i) - x(i-1)).
				double tempM = fabs((*itCurr).getValue() - (*itPrev).getValue()) / ((*itCurr).getX() - (*itPrev).getX());
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
	for (std::size_t i = 0; i < paramsZ_.size(); ++i) {
		if (i == maxIndex_) {
			paramsZ_[i] = bestTrial.getValue();
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

	std::set<PointTrial>::const_iterator itPrev = trials_.cbegin();
	std::set<PointTrial>::const_iterator itCurr = ++trials_.cbegin();

	for (; itCurr != trials_.cend() && itPrev != --trials_.end(); ++itCurr, ++itPrev) {
		// Предыдущая точка x(i-1).
		PointTrial pointPrev = *itPrev;
		// Текущая точка x(i).
		PointTrial pointCurr = *itCurr;	

		double currR = 0;
		double delta = pointCurr.getX() - pointPrev.getX();

		if (pointPrev.getIndex() == pointCurr.getIndex()) {
			int index = pointPrev.getIndex();
			currR = delta
				+ pow(pointCurr.getValue() - pointPrev.getValue(), 2) / (delta * pow(paramR_, 2) * pow(maxValuesDifference_[index], 2)) 
				- 2 * (pointCurr.getValue() + pointPrev.getValue() - 2 * paramsZ_[index]) / (paramR_ * maxValuesDifference_[index]);
		}
		else if (pointPrev.getIndex() < pointCurr.getIndex()) {
			int index = pointCurr.getIndex();
			currR = 2 * delta - 4 * (pointCurr.getValue() - paramsZ_[index]) / (paramR_ * maxValuesDifference_[index]);
		}
		else {
			int index = pointPrev.getIndex();
			currR = 2 * delta - 4 * (pointPrev.getValue() - paramsZ_[index]) / (paramR_ * maxValuesDifference_[index]);
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
	for (std::size_t i = 1; i < funcs_.size(); ++i) {
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
			- (interval[0].getValue() - interval[1].getValue()) / (2 * paramR_ * maxValuesDifference_[index]);
		return newTrial(newX);
	}
}



