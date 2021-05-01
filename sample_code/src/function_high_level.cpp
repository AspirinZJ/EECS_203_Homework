/** @file       function_high_level.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/01/2021
 *  @brief      Default value, place holder, function reload, ... function,
 *  @details    
 *  @note       
 */
#include <iostream>

// default value
int mySum(int a, int b, int c = 3)
{
	return a + b + c;
}

// place holder
int fun1(int a, int, int)
{
	std::cout << "a: " << a << std::endl;
}

// !!! function reload !!!
// 1. 同一作用域 :: 同一类或命名空间
// 2. 同一函数名
// 3. 不同参数列表 : 参数类型不同， 参数个数不同， 参数顺序不同
// 4. 返回类型必须相同

double myMax(int a, int b)
{
	std::cout << "int, int" << std::endl;
	return a > b ? a : b;
}

double myMax(double a, double b) // 类型不同
{
	return a > b ? a : b;
}

double myMax(double a, double b, double c) // 个数不同
{
	double tmp = a > b ? a : b;
	return tmp > c ? tmp : c;
}

double myMax(int a, double b)
{
	double aDouble = static_cast<double>(a);
	return aDouble > b ? aDouble : b;
}

double myMax(double b, int a)
{
	double aDouble = static_cast<double>(a);
	return aDouble > b ? aDouble : b;
}

double myMax(int a, int &b)
{
	return a > b ? a : b;
}


int main()
{
	// std::cout << mySum(1, 2, 4) << std::endl;
	// std::cout << mySum(1, 2) << std::endl;

	// fun1(1, 2, 3);

	std::cout << myMax(1.0, 2.0) << std::endl;
	std::cout << myMax(1, 2.0) << std::endl;
	std::cout << myMax(1.0, 2) << std::endl;
	std::cout << myMax(1.0, 2.0, 3.0) << std::endl;
	std::cout << myMax(1, 2, 3) << std::endl;
	std::cout << myMax(1, 2) << std::endl;


	return 0;
}