/** @file       object_properties2.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/08/2021
 *  @brief      构造函数调用规则
 *  @details
 *  @note       
 */
#include <iostream>

class Person
{
public:
	// 编yi器会自动提供默认构造，如果什么都没有
	// Person() { std::cout << "default constructor" << std::endl; }
	// Person(int age, const char *name) : m_age(age), m_name(name) {}
	Person(const Person &person) : m_age(person.m_age), m_name(person.m_name){}


public:
	int m_age;
	std::string m_name;
};

int main()
{
	// "Jack" => const char *
	// Person person(20, "Jack");
	// Person person1 = person;
	// Person person;

	return 0;
}
