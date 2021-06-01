/** @file       base_usage.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/23/2021
 *  @brief      语法 class 派生类名 ： 继承方式 基类名 {}
 *  @details    
 *  @note       
 */
#include <iostream>

class Person
{
public:
	void fun1() { std::cout << "Person" << std::endl;}
	int m_age;
	std::string m_name;
};

class Teacher : public Person
{
public:
	void fun1() { std::cout << "Teacher func1" << std::endl;}
	void fun2() { std::cout << "Teacher" << std::endl;}
	int m_id;
};

class Student : public Person
{
public:
	void fun3() { std::cout << "Student" << std::endl;}
	int m_stdNumber;
};

int main()
{
	Teacher teacher;
	teacher.fun2();

	return 0;
}

