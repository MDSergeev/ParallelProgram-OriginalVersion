#pragma once
#include "PointTrial.h"
#include <vector>
#include <stdexcept>
#include <functional>

typedef std::vector<std::function<double(double)>> Funcs;

/// <summary>
/// ����� ��� ����������� ������ �����������.
/// </summary>
class GridSearchMethod {
public:
	/// <summary>
	/// �������������� ����� ������ ������ GridSearchMethod.
	/// </summary>
	/// <param name="leftBound">����� ����� �������.</param>
	/// <param name="rightBound">������ ����� �������.</param>
	/// <param name="eps">�������� ������.</param>
	/// <param name="funcs">������� ����������� � ������� �������.</param>
	GridSearchMethod(double leftBound, double rightBound, double eps, const Funcs& funcs);

	/// <summary>
	/// ��������� ������� ������.
	/// </summary>
	/// <returns></returns>
	PointTrial Run();

private:
	// ����� ����� �������.
	double leftBound_;

	// ������ ����� �������.
	double rightBound_;

	// �������� �������� ������.
	double eps_;

	// ������� ����������� � ������� �������.
	Funcs funcs_;

	// ���������� ��������� � ����� x.
	PointTrial newTrial(double x);
};