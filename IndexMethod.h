#include <vector>
#include <set>
#include <functional>
#include "PointTrial.h"

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
	std::vector<double> paramsR_;

	// Упорядоченное множество точек, в которых производились испытания.
	std::set<PointTrial> trials_;

	// Точки с фиксированным индексом (множество I).
	std::vector<std::set<PointTrial>> fixedIndex_;

	// Нижние границы для каждого индекса (множество mu(v)).
	std::vector<double> lowerBounds_;

	// Параметр Z алгоритма.
	std::vector<double> paramsZ_;

	// Максимальный индекс.
	int maxIndex_;


};