class PointTrial {
public:
	explicit PointTrial(double x, double value, int index);

	PointTrial(const PointTrial& copy);

	PointTrial& operator= (const PointTrial& pointTrial);

	double getX();

	double getValue();

	int getIndex();

	friend bool operator< (const PointTrial& lhs, const PointTrial& rhs);

	friend bool operator> (const PointTrial& lhs, const PointTrial& rhs);

private:
	double x_;
	double value_;
	int index_;
};