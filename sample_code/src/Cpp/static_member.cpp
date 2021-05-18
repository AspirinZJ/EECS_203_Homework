/** @file       static_member.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/12/2021
 *  @brief      
 *  @details    
 *  @note       静态成员变量 1. 所有对象共享一份数据 2. 在编译阶段分配在内存的全局区 3.类内声明， 类外初始化
 */
#include <iostream>

class Person
{
public:
	static void showPerson() {std::cout << m_age << std::endl;}
	static int m_age; // 静态成员变量
	std::string m_name;
};

int Person::m_age = 20;

int main()
{
	Person person;
	// person.m_age = 25;

	Person person1;
	// person1.m_age = 16;

	// person.showPerson();
	Person::showPerson();

	return 0;
}
