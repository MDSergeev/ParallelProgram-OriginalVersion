#pragma once

class PointTrial {
public:
	explicit PointTrial(double x, double value, int index);

	PointTrial(const PointTrial& copy);

	double x() const;

	double value() const;

	int index() const;

	PointTrial& operator= (const PointTrial& pointTrial);

	friend bool operator< (const PointTrial& lhs, const PointTrial& rhs);

	friend bool operator> (const PointTrial& lhs, const PointTrial& rhs);

private:
	double x_;
	double value_;
	int index_;
};