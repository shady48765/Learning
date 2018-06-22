#include <iostream>
#include <cmath>
#include <vector>

double calculator(double num1, double num2)
{
    return (num1 + num2);
}

double calculator(double num1, int num2)
{
    double sum = pow(num1, num2);
    return sum;
}



// vector push and printf
int main(void)
{
	using namespace std;

	vector<int> arryint;

	for(int counter = 0; counter < 10; counter++) {

		arryint.push_back( counter * pow(2, counter));
	}

	for(int counter = 0; counter < arryint.size(); counter++){
		cout << arryint[counter] << ", ";
	}
	cout << endl;

	return 0;
}



#if 0
int main(void) {
    using namespace std;

    double a = 10;
    double b = 2;
    int c = 3;
    double sum = calculator(a, b);
    cout << "sum1 = " << sum << endl;

    sum = calculator(a, c);
    cout << "sum2 = " << sum << endl;

    return 0;
}
#endif