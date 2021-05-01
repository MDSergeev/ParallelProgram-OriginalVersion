#pragma once
#include "PointTrial.h"
#include <vector>
#include <set>
#include <stdexcept>
#include <functional>

typedef std::vector<std::function<double(double)>> Funcs;


class IndexMethod {
public:
	/// <summary>
	/// Инициализирует новый объект класса IndexMethod.
	/// </summary>
	/// <param name="leftBound">Левый конец отрезка.</param>
	/// <param name="rightBound">Правый конец отрезка.</param>
	/// <param name="eps">Точность поиска.</param>
	/// <param name="paramR">Параметр надежности.</param>
	/// <param name="funcs">Функции ограничения и целевая функция.</param>
	IndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs);

	/// <summary>
	/// Запускает процесс происка.
	/// </summary>
	/// <returns></returns>
	PointTrial Run();

private:
	// Левый конец отрезка.
	double leftBound_;

	// Правый конец отрезка.
	double rightBound_;

	// Заданная точность поиска.
	double eps_;

	// Функции ограничения и целевая функция.
	Funcs funcs_;

	// Параметр надёжности R.
	double paramR_;

	// Упорядоченное множество точек, в которых производились испытания.
	std::set<PointTrial> trials_;

	// Точки с фиксированным индексом (множество I).
	std::vector<std::set<PointTrial>> fixedIndex_;

	// Максимальные значения относительных первых разностей (множество mu(v)).
	std::vector<double> maxValuesDifference_;

	// Параметр Z* алгоритма.
	std::vector<double> paramsZ_;

	// Максимальный индекс.
	int maxIndex_;

	// Определение точек с фиксированным индексом (множества I).
	// Правило 2.
	void calculateFixedIndex();

	// Определение максимальных значений относительных первых разностей (множество mu(v)).
	// Правило 3.
	void calculateMaxValuesDifference();

	// Определение параметра Z алгоритма.
	// Правило 4.
	void calculateZ(const PointTrial& bestTrial);

	// Поиск интервала с максимальной характеристикой R.
	// Правило 5-7.
	std::vector<PointTrial> calculateMaxR();

	// Проведение испытания в точке x.
	PointTrial newTrial(double x);

	// Проведение нового испытания для интервала interval.
	// Правило 8.
	PointTrial newTrial(const std::vector<PointTrial>& interval);
};