#include "ParallelIndexMethod.h"

ParallelIndexMethod::ParallelIndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs, int N) :
	IndexMethod(leftBound, rightBound, eps, paramR, funcs) {

	// Заносим граничные точки.
	trials_.insert(PointTrial(leftBound_, NULL, -1));
	trials_.insert(PointTrial(rightBound_, NULL, -1));

	N_ = N;
	maxIndex_ = -1;
	isStop = false;
}

PointTrial ParallelIndexMethod::Run()
{
	// TODO
}

void ParallelIndexMethod::threadRun(double startPoint)
{
	mtx1_.lock();
	// Получаем множество точек для текущего потока.
	std::set<PointTrial> threadTrials(trials_);

	// Проводим первое испытание в заданной точке.
	PointTrial startTrial = newTrial(startPoint);
	// Заносим испытание в множество.
	threadTrials.insert(startTrial);
	trials_.insert(startTrial);

	if ((startTrial.index() > maxIndex_)
		|| (startTrial.index() == maxIndex_ && startTrial.value() < bestTrial_.value())) {
		bestTrial_ = startTrial;
		maxIndex_ = startTrial.index();
	}
	
	mtx1_.unlock();

	while (!isStop) {
		// TODO
	}
}

std::vector<std::set<PointTrial>> ParallelIndexMethod::calculateFixedIndex(const std::set<PointTrial>& threadTrials)
{
	std::vector<std::set<PointTrial>> fixedIndex(funcs_.size());

	for (std::set<PointTrial>::const_iterator it = ++threadTrials.cbegin(); it != --threadTrials.cend(); ++it) {
		fixedIndex[(*it).index()].insert(*it);
	}

	return fixedIndex;
}

std::vector<double> ParallelIndexMethod::calculateMaxValuesDifference(const std::vector<std::set<PointTrial>>& threadFixedIndex)
{
	std::vector<double> maxValuesDifference(funcs_.size());

	// Внешний цикл - по каждому номеру индекса.
	for (std::size_t i = 0; i < threadFixedIndex.size(); ++i) {
		if (threadFixedIndex[i].size() < 2) {
			maxValuesDifference[i] = 1;
		}
		else {
			double maxM = -HUGE_VAL;

			// Итератор на предыдущий элемент множества.
			std::set<PointTrial>::const_iterator itPrev = threadFixedIndex[i].cbegin();
			// Итератор на текущий элемент множества.
			std::set<PointTrial>::const_iterator itCurr = threadFixedIndex[i].cbegin();
			for (; itPrev != --threadFixedIndex[i].cend() && itCurr != threadFixedIndex[i].cend(); ++itPrev, ++itCurr) {
				// Вычисляем |z(i) - z(i-1)| / (x(i) - x(i-1)).
				double tempM = fabs((*itCurr).value() - (*itPrev).value()) / ((*itCurr).x() - (*itPrev).x());
				if (tempM > maxM) {
					maxM = tempM;
				}
			}
			if (maxM > 0) {
				maxValuesDifference[i] = maxM;
			}
			else {
				maxValuesDifference[i] = 1;
			}
		}
	}

	return maxValuesDifference;
}

std::vector<double> ParallelIndexMethod::calculateZ(const PointTrial& bestTrial)
{
	std::vector<double> paramsZ(funcs_.size());

	for (std::size_t i = 0; i < paramsZ.size(); ++i) {
		if (i == bestTrial.index()) {
			paramsZ[i] = bestTrial.value();
		}
		else {
			paramsZ[i] = 0;
		}
	}

	return paramsZ;
}

std::vector<PointTrial> ParallelIndexMethod::calculateMaxR(const std::set<PointTrial>& threadTrials,
	const std::vector<double>& threadMaxValuesDifference, const std::vector<double>& threadParamsZ)
{
	PointTrial bestPrev = PointTrial(NULL, NULL, -1);
	PointTrial bestCurr = PointTrial(NULL, NULL, -1);
	double maxR = -HUGE_VAL;

	std::set<PointTrial>::const_iterator itPrev = threadTrials.cbegin();
	std::set<PointTrial>::const_iterator itCurr = ++threadTrials.cbegin();

	for (; itCurr != threadTrials.cend() && itPrev != --threadTrials.end(); ++itCurr, ++itPrev) {
		// Предыдущая точка x(i-1).
		PointTrial pointPrev = *itPrev;
		// Текущая точка x(i).
		PointTrial pointCurr = *itCurr;

		double currR = 0;
		double delta = pointCurr.x() - pointPrev.x();

		if (pointPrev.index() == pointCurr.index()) {
			int index = pointPrev.index();
			currR = delta
				+ pow(pointCurr.value() - pointPrev.value(), 2) / (delta * pow(paramR_, 2) * pow(threadMaxValuesDifference[index], 2))
				- 2 * (pointCurr.value() + pointPrev.value() - 2 * threadParamsZ[index]) / (paramR_ * threadMaxValuesDifference[index]);
		}
		else if (pointPrev.index() < pointCurr.index()) {
			int index = pointCurr.index();
			currR = 2 * delta - 4 * (pointCurr.value() - threadParamsZ[index]) / (paramR_ * threadMaxValuesDifference[index]);
		}
		else {
			int index = pointPrev.index();
			currR = 2 * delta - 4 * (pointPrev.value() - threadParamsZ[index]) / (paramR_ * threadMaxValuesDifference[index]);
		}

		if (currR > maxR) {
			maxR = currR;
			bestPrev = pointPrev;
			bestCurr = pointCurr;
		}
	}

	return std::vector<PointTrial>({ bestPrev, bestCurr });
}

PointTrial ParallelIndexMethod::newTrial(double x)
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

PointTrial ParallelIndexMethod::newTrial(const std::vector<PointTrial>& interval, const std::vector<double>& threadMaxValuesDifference)
{
	if (interval[0].index() != interval[1].index()) {
		double newX = (interval[0].x() + interval[1].x()) / 2;
		return newTrial(newX);
	}
	else {
		int index = interval[0].index();
		double newX = (interval[0].x() + interval[1].x()) / 2
			- (interval[0].value() - interval[1].value()) / (2 * paramR_ * threadMaxValuesDifference[index]);
		return newTrial(newX);
	}
}

