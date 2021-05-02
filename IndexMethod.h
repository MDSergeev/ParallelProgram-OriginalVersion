#pragma once
#include "PointTrial.h"
#include <vector>
#include <stdexcept>
#include <functional>

typedef std::vector<std::function<double(double)>> Funcs;

class IndexMethod {
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="leftBound">����� ����� �������.</param>
	/// <param name="rightBound">������ ����� �������.</param>
	/// <param name="eps">�������� ������.</param>
	/// <param name="paramR">�������� ����������.</param>
	/// <param name="funcs">������� ����������� � ������� �������.</param>
	IndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs);

	/// <summary>
	/// ��������� ������� ������ �������.
	/// </summary>
	/// <returns></returns>
	virtual PointTrial Run() = 0;

protected:
	// ����� ����� �������.
	double leftBound_;

	// ������ ����� �������.
	double rightBound_;

	// �������� �������� ������.
	double eps_;

	// ������� ����������� � ������� �������.
	Funcs funcs_;

	// �������� ��������� R.
	double paramR_;
};