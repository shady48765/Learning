// test_vector.cpp: 定义控制台应用程序的入口点。
//

#include <iostream>
#include <vector>

// 运算符重载(Java 里的多态)
int func(int a, int b)
{
	return (a + b);
}

int func(double a, double b)
{
	return (int)(a * b);
}
// -----------------------------------------------------

int main()
{
	using namespace std;

	// input test
	cout << "Please in put a interge number: ";
	int inputTemp;
	cin >> inputTemp;
	cout << "input is : " << inputTemp << endl;
	// ----------------------------------------------------------
	// vector test
	vector<int>arr_vector;
	for (int i = 0; i < 10; i++) {
		arr_vector.push_back(i * 10);
	}
	for (int i = 0; i < 10; i++) {
		cout << arr_vector[i] << ", ";
	}
	cout << endl;
	// ----------------------------------------------------------

	int sum1 = func(5, 5);
	double sum2 = func(5.00, 5.00);
	cout << "sum1 = " << sum1 << endl;
	cout << "sum2 = " << sum2 << endl;


	system("pause");

	return 0;
}

