#include <iostream>
#include "demo.h"

int main(void)
{
	std::cout << "---> init --->" << std::endl;
	Solution solution;
	int answer = 0;
	solution.feb(10, answer);
	std::cout << "---> answer = "
			<< answer << std::endl;
	return 0;
}

int Solution::feb(int number, int& answer)
{
	if(number == 0) {
		return answer;
	} else {
		answer += (number * 2);
		number--;
        std::cout << "---> number = " << number
                << ", answer = " << answer
                << std::endl;
		return Solution::feb(number, answer);
	}
}
