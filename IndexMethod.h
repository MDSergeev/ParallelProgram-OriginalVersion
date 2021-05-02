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
	/// <param name="leftBound">Левый конец отрезка.</param>
	/// <param name="rightBound">Правый конец отрезка.</param>
	/// <param name="eps">Точность поиска.</param>
	/// <param name="paramR">Параметр надежности.</param>
	/// <param name="funcs">Функции ограничения и целевая функция.</param>
	IndexMethod(double leftBound, double rightBound, double eps, double paramR, const Funcs& funcs);

	/// <summary>
	/// Запускает процесс поиска решения.
	/// </summary>
	/// <returns></returns>
	virtual PointTrial Run() = 0;

protected:
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
};