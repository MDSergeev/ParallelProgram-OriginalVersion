#include <vector>
#include <functional>

typedef std::vector<std::function<double(double)>> Funcs;


class IndexMethod {
public:
	/// <summary>
	/// �������������� ����� ������ ������ IndexMethod.
	/// </summary>
	/// <param name="leftBound">����� ����� �������.</param>
	/// <param name="rightBound">������ ����� �������.</param>
	/// <param name="eps">�������� ������.</param>
	/// <param name="funcs">������� ����������� � ������� �������.</param>
	IndexMethod(double leftBound, double rightBound, double eps, const Funcs& funcs);



private:
	double leftBound_;
	double rightBound_;
	double eps_;
	Funcs funcs_;
};