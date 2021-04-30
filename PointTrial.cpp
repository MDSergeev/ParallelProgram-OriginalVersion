#include "PointTrial.h"

PointTrial::PointTrial(double x, double value, int index) :
	x_(x), value_(value), index_(index) {}

PointTrial::PointTrial() : 
	x_(NULL), value_(NULL), index_(-1) {}

PointTrial::PointTrial(const PointTrial& copy) :
	x_(copy.x_), value_(copy.value_), index_(copy.index_) {}

double PointTrial::getX() const {
	return x_;
}

double PointTrial::getValue() const {
	return value_;
}

int PointTrial::getIndex() const {
	return index_;
}

PointTrial& PointTrial::operator=(const PointTrial& pointTrial) {
	// Проверка на самоприсваивание.
	if (this == &pointTrial) {
		return *this;
	}

	x_ = pointTrial.x_;
	value_ = pointTrial.value_;
	index_ = pointTrial.index_;

	return *this;
}

bool operator<(const PointTrial& lhs, const PointTrial& rhs) {
	return lhs.x_ < rhs.x_;
}

bool operator>(const PointTrial& lhs, const PointTrial& rhs) {
	return lhs.x_ > rhs.x_;
}