/** @file       static_member.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/12/2021
 *  @brief      
 *  @details    
 *  @note       静态成员变量 1. 所有对象共享一份数据 2. 在编译阶段分配在内存的全局区 3.类内声明， 类外初始化
 */
#include <iostream>
#include <vector>

class Person
{
public:
	static void showPerson() {std::cout << m_age << std::endl;}

	static void setMAge(int mAge);
	void setMName(const std::string &mName);
	static int m_age; // 静态成员变量
	std::string m_name;
	const std::string &getMName() const;
	int getMAge() const;
	double getMHeight() const;
	const std::vector<int> &getVec() const;
};

int Person::m_age = 20;

const std::string &Person::getMName() const
{
	return m_name;
}

void Person::setMAge(int mAge)
{
	m_age = mAge;
}

void Person::setMName(const std::string &mName)
{
	m_name = mName;
}

int Person::getMAge() const
{
	return m_age;
}

double Person::getMHeight() const
{
	double m_height;
	return m_height;
}

const std::vector<int> &Person::getVec() const
{
	return vec;
}

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
