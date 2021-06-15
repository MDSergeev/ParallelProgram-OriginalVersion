#pragma once
#include "IndexMethod.h"

/// <summary>
/// ���������������� ��������� ����� �����������.
/// </summary>
class OriginalIndexMethod : public IndexMethod {
public:

	/// <summary>
	/// �������������� ����� ������ ������ OriginalIndexMethod.
	/// </summary>
	/// <param name="leftBound">����� ����� �������.</param>
	/// <param name="rightBound">������ ����� �������.</param>
	/// <param name="eps">�������� ������.</param>
	/// <param name="paramR">�������� ����������.</param>
	/// <param name="funcs">������� ����������� � ������� �������.</param>
	OriginalIndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs);

	/// <summary>
	/// ��������� ������� �������.
	/// </summary>
	/// <returns></returns>
	PointTrial Run() override;

private:

	// ������������� ��������� �����, � ������� ������������� ���������.
	std::set<PointTrial> trials_;

	// ����� � ������������� �������� (��������� I).
	std::vector<std::set<PointTrial>> fixedIndex_;

	// ������������ �������� ������������� ������ ��������� (��������� mu(v)).
	std::vector<double> maxValuesDifference_;

	// �������� Z* ���������.
	std::vector<double> paramsZ_;

	// ������������ ������.
	int maxIndex_;

	// ����������� ����� � ������������� �������� (��������� I).
	// ������� 2.
	void calculateFixedIndex();

	// ����������� ������������ �������� ������������� ������ ��������� (��������� mu(v)).
	// ������� 3.
	void calculateMaxValuesDifference();

	// ����������� ��������� Z ���������.
	// ������� 4.
	void calculateZ(const PointTrial& bestTrial);

	// ����� ��������� � ������������ ��������������� R.
	// ������� 5-7.
	std::vector<PointTrial> calculateMaxR();

	// ���������� ��������� � ����� x.
	PointTrial newTrial(double x);

	// ���������� ������ ��������� ��� ��������� interval.
	// ������� 8.
	PointTrial newTrial(const std::vector<PointTrial>& interval);
};