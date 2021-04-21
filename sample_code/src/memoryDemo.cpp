/** @file       内存分区模型.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/21/2021
 *  @brief      
 *  @details   ==代码区==：存放函数体的二进制代码， 由操作系统管理
==全局区==：存放全局变量，静态变量和常量
==栈区==： 由编译器自动分配释放，存放函数的参数值，局部变量等
==堆区==： 由程序员分配和释放，若程序员不释放，程序结束时由操作系统回收
 *  @note       
 */

#include <iostream>


int a = 3;

void test(int a)
{
	int b = 5;
	b++;
	std::cout << b << std::endl;
}

void test2()
{
	int *pInt = (int *) malloc(sizeof(int));
	*pInt = 5;
	std::cout << *pInt << std::endl;

	free(pInt);

	// 类型 *变量名 = new 类型（数据）;
	int *pInt2 = new int(5);
	std::cout << *pInt2 << std::endl;
	delete pInt2;
}

// 不能返回局部变量的地址
int *getAddr()
{
	int *p = new int(5);
	return p;
}

int main()
{
	int *p = getAddr();
	std::cout << *p << std::endl;

	// new array
	int *pInt = new int(100);
	int *pInt_ = (int *) malloc(sizeof(int));

	int *arr = new int[5]{};
	int *arr_ = (int *) malloc(sizeof(int) * 5);


	for (int i = 0; i < 5; ++i)
	{
		*(arr + i) = i;
		std::cout << *(arr + i) << " ";
	}
	std::cout << std::endl;

	int a(100);
	std::cout << "a : " << a << std::endl;

	return 0;
}