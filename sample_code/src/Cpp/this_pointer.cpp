/** @file       this_pointer.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/12/2021
 *  @brief      
 *  @details
 *  @note       C++类的成员变量和成员函数分开存储，只有非静态成员变量才属于类的对象上
 *  			C++空对象占用空间为1
 */

#include <iostream>

class Person
{
public:
	Person(int age, const char *name)
	{
		m_age = age;
		m_name = name;
	}

	Person &addPerson(const Person &person) {m_age += person.m_age; return *this;}

	void show() { std::cout << this->m_age << std::endl; }

public:
	int m_age;
	std::string m_name;
};


int main()
{
	//	Person person{}; // 空对象
	//	Person person2;
	//	std::cout << sizeof(person) << std::endl << sizeof(person2) << std::endl;
	Person jack(25, "Jack");
	Person alice(20, "Alice");
	Person bob(15, "Bob");
	jack.addPerson(alice).addPerson(bob);

	std::cout << jack.m_age << std::endl;

	return 0;
}