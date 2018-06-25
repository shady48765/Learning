#include <iostream>

class Calc
{
private:
	int numb1;
	int numb2;
public:
	Calc();
	~Calc();

	int add(int param1, int param2);


};
Calc::Calc()
{
	std::cout << "--- check ---" << std::endl;
}
Calc::~Calc()
{
	std::cout << "end" << std::endl;
}
int Calc::add(int param1, int param2)
{
	return (param1 + param2);
}

int main(void)
{
	{
		Calc calc;
		int sum = calc.add(10, 10);

		std::cout << "----- Start ------";
		std::cout << "\n";
		std::cout << "sum = ";
		std::cout << sum;
		std::cout << "\n";
	}

	return 0;
}


