#include <vector>
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
	/// <param name="funcs">Функции ограничения и целевая функция.</param>
	IndexMethod(double leftBound, double rightBound, double eps, const Funcs& funcs);



private:
	double leftBound_;
	double rightBound_;
	double eps_;
	Funcs funcs_;
};