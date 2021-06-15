#pragma once
#include "IndexMethod.h"
#include <thread>
#include <mutex>

/// <summary>
/// ������������ ��������� ����� �����������.
/// </summary>
class ParallelIndexMethod : public IndexMethod {
public:

	/// <summary>
	/// �������������� ����� ������ ������ ParallelIndexMethod.
	/// </summary>
	/// <param name="leftBound">����� ����� �������.</param>
	/// <param name="rightBound">������ ����� �������.</param>
	/// <param name="eps">�������� ������.</param>
	/// <param name="paramR">�������� ����������.</param>
	/// <param name="funcs">������� ����������� � ������� �������.</param>
	/// <param name="N">���������� �������.</param>
	ParallelIndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs, int N);

	/// <summary>
	/// ��������� ������� ������.
	/// </summary>
	/// <returns></returns>
	PointTrial Run() override;

private:

	// ������������� ��������� �����, � ������� ������������� ���������.
	std::set<PointTrial> trials_;

	// ����� �������.
	int N_;

	// ������������ ������.
	int maxIndex_;

	// ��������� ���������.
	PointTrial bestTrial_ = PointTrial(leftBound_, NULL, -1);

	// ������.
	std::vector<std::thread> threads_;

	// ��������.
	std::mutex mtx1_, mtx2_;

	// ���� ��������� ����������.
	bool isStop;

	// ������� ��� ������� ������.
	void threadRun(double startPoint);

	// ����������� ����� � ������������� �������� � ���������� ������ (��������� I).
	std::vector<std::set<PointTrial>> calculateFixedIndex(const std::set<PointTrial>& threadTrials);

	// ����������� ������������ �������� ������������� ������ ��������� � ���������� ������ (��������� mu(v)).
	std::vector<double> calculateMaxValuesDifference(const std::vector<std::set<PointTrial>>& threadFixedIndex);

	// ���������� ��������� Z* ���������.
	std::vector<double> calculateZ(const PointTrial& bestTrial);

	// ����� ��������� � ������������ ��������������� R.
	std::vector<PointTrial> calculateMaxR(const std::set<PointTrial>& threadTrials,
		const std::vector<double>& threadMaxValuesDifference, const std::vector<double>& threadParamsZ);

	// ���������� ��������� � ����� x.
	PointTrial newTrial(double x);

	// ���������� ������ ��������� ��� ��������� interval.
	// ������� 8.
	PointTrial newTrial(const std::vector<PointTrial>& interval, const std::vector<double>& threadMaxValuesDifference);
};