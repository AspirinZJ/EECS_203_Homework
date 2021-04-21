/** @file       referenceDemo.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/21/2021
 *  @brief      
 *  @details    
 *  @note       
 */

#include <iostream>

class MyClass
{
public:
	MyClass() = default;
	MyClass(int num) : mNum(num) {}
	int mNum;
};

void swap(int a , int b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

void swap2(int &ref1, int &ref2)
{
	int tmp = ref1;
	ref1 = ref2;
	ref2 = tmp;
}

int &testRef()
{
	static int test = 100;
	std::cout << "test in funtion: " << test << std::endl;
	return test;
}

int main()
{

	testRef() = 90;
	std::cout << "test in main: " << testRef() << std::endl;
	testRef() = 90;
	std::cout << "test in main: " << testRef() << std::endl;



	return 0;
}