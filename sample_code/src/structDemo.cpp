/** @file       structDemo.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/21/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>

// 从创建struct
struct MyStruct
{
	int num1;
	int num2;
	std::string str;
};

void demo1()
{
	struct MyStruct myStruct1 = {1, 2, "str"};
	std::cout << myStruct1.str << std::endl;

	// struct array
	MyStruct myStructArr[5];
	myStructArr[0] = {1, 2, "str"};
	myStructArr[1] = {1, 2, "str"};
	myStructArr[2] = {1, 2, "str"};
	myStructArr[3] = {1, 2, "str"};
	myStructArr[4] = {1, 2, "str"};

	MyStruct *pMyStruct = &myStruct1;
	std::cout << (*pMyStruct).num1 << std::endl;
	std::cout << pMyStruct->num1 << std::endl;

	struct MyStruct2
	{
		int num1;
		MyStruct myStruct;
	};
}

void printMyStruct(MyStruct myStruct)
{
	myStruct.num1 = 100;
}

void printMyStruct2(const MyStruct *pMyStruct)
{
	std::cout << pMyStruct->num1 << '\t' << pMyStruct->num2 << '\t' << pMyStruct->str << std::endl;
}

void printMyStruct3(MyStruct &refMyStruct)
{
	std::cout << refMyStruct.num1 << '\t' << refMyStruct.num2 << '\t' << refMyStruct.str << std::endl;
}


int main()
{
	MyStruct myStruct = {1, 2, "test_str"};
	printMyStruct(myStruct);

	return 0;
}