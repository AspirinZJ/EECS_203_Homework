/** @file       object_properties.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/01/2021
 *  @brief      构造函数和析构函数
 *  @details    构造函数和类型一致， 没有返回值， 不能写void
 *  @note       拷贝构造函数的调用时机: 1. 拷贝构造时， 2. 值传递对象给函数时, 3. 以值传递返回局部对象
 */
#include <iostream>

// 构造函数的类型
// 有参构造和无参构造
// 普通构造，拷贝构造和转换构造
class Person
{
public:
	Person()
	{
		// std::cout << "Default constructor." << std::endl;
	}  // 默认构造 无参构造
	// Person(int age) : m_age(age) {}

	// explicit 禁用转换构造函数
	explicit Person(int age) // 转换构造函数 => 不包含explicit关键字的单参构造函数
	{
		m_age = age;
		std::cout << "有参构造" << std::endl;
	}

	// 拷贝构造
	Person(const Person &person) : m_age(person.m_age) { std::cout << "copy constructor" << std::endl; }

	// ~Person() { std::cout << "destructor" << std::endl; }

public:
	int m_age;
};

void test1(Person &person)
{
	person.m_age = 100;
}

Person test2()
{
	Person person(50);
	return person;
}

int main()
{
	//	Person person(20);
	//	// Person person = 20;
	//	// Person person1 = person;
	//
	//	test1(person);
	//	std::cout << person.m_age << std::endl;
	Person person = test2();


	return 0;
}