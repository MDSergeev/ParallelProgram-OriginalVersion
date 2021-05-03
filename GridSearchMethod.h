#pragma once
#include "PointTrial.h"
#include <vector>
#include <stdexcept>
#include <functional>

typedef std::vector<std::function<double(double)>> Funcs;

/// <summary>
/// Класс для переборного метода оптимизации.
/// </summary>
class GridSearchMethod {
public:
	/// <summary>
	/// Инициализирует новый объект класса GridSearchMethod.
	/// </summary>
	/// <param name="leftBound">Левый конец отрезка.</param>
	/// <param name="rightBound">Правый конец отрезка.</param>
	/// <param name="eps">Точность поиска.</param>
	/// <param name="funcs">Функции ограничения и целевая функция.</param>
	GridSearchMethod(double leftBound, double rightBound, double eps, const Funcs& funcs);

	/// <summary>
	/// Запускает процесс поиска.
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

	// Проводение испытания в точке x.
	PointTrial newTrial(double x);
};