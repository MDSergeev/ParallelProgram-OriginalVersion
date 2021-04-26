#include <vector>
#include <set>
#include <functional>
#include "PointTrial.h"

typedef std::vector<std::function<double(double)>> Funcs;


class IndexMethod {
public:
	/// <summary>
	/// �������������� ����� ������ ������ IndexMethod.
	/// </summary>
	/// <param name="leftBound">����� ����� �������.</param>
	/// <param name="rightBound">������ ����� �������.</param>
	/// <param name="eps">�������� ������.</param>
	/// <param name="paramR">�������� ����������.</param>
	/// <param name="funcs">������� ����������� � ������� �������.</param>
	IndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs);


private:
	// ����� ����� �������.
	double leftBound_;

	// ������ ����� �������.
	double rightBound_;

	// �������� �������� ������.
	double eps_;

	// ������� ����������� � ������� �������.
	Funcs funcs_;

	// �������� ��������� R.
	std::vector<double> paramsR_;

	// ������������� ��������� �����, � ������� ������������� ���������.
	std::set<PointTrial> trials_;

	// ����� � ������������� �������� (��������� I).
	std::vector<std::set<PointTrial>> fixedIndex_;

	// ������ ������� ��� ������� ������� (��������� mu(v)).
	std::vector<double> lowerBounds_;

	// �������� Z ���������.
	std::vector<double> paramsZ_;

	// ������������ ������.
	int maxIndex_;


};