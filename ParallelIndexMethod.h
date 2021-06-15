#pragma once
#include "IndexMethod.h"
#include <thread>
#include <mutex>

/// <summary>
/// Параллельный индексный метод оптимизации.
/// </summary>
class ParallelIndexMethod : public IndexMethod {
public:

	/// <summary>
	/// Инициализирует новый объект класса ParallelIndexMethod.
	/// </summary>
	/// <param name="leftBound">Левый конец отрезка.</param>
	/// <param name="rightBound">Правый конец отрезка.</param>
	/// <param name="eps">Точность поиска.</param>
	/// <param name="paramR">Параметр надежности.</param>
	/// <param name="funcs">Функции ограничения и целевая функция.</param>
	/// <param name="N">Количество потоков.</param>
	ParallelIndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs, int N);

	/// <summary>
	/// Запускает процесс поиска.
	/// </summary>
	/// <returns></returns>
	PointTrial Run() override;

private:

	// Упорядоченное множество точек, в которых производились испытания.
	std::set<PointTrial> trials_;

	// Число потоков.
	int N_;

	// Максимальный индекс.
	int maxIndex_;

	// Наилучшее испытание.
	PointTrial bestTrial_ = PointTrial(leftBound_, NULL, -1);

	// Потоки.
	std::vector<std::thread> threads_;

	// Мьютексы.
	std::mutex mtx1_, mtx2_;

	// Флаг окончания вычислений.
	bool isStop;

	// Функция для запуска потока.
	void threadRun(double startPoint);

	// Определение точек с фиксированным индексом в конкретном потоке (множества I).
	std::vector<std::set<PointTrial>> calculateFixedIndex(const std::set<PointTrial>& threadTrials);

	// Определение максимальных значений относительных первых разностей в конкретном потоке (множество mu(v)).
	std::vector<double> calculateMaxValuesDifference(const std::vector<std::set<PointTrial>>& threadFixedIndex);

	// Вычисление параметра Z* алгоритма.
	std::vector<double> calculateZ(const PointTrial& bestTrial);

	// Поиск интервала с максимальной характеристикой R.
	std::vector<PointTrial> calculateMaxR(const std::set<PointTrial>& threadTrials,
		const std::vector<double>& threadMaxValuesDifference, const std::vector<double>& threadParamsZ);

	// Проведение испытания в точке x.
	PointTrial newTrial(double x);

	// Проведение нового испытания для интервала interval.
	// Правило 8.
	PointTrial newTrial(const std::vector<PointTrial>& interval, const std::vector<double>& threadMaxValuesDifference);
};