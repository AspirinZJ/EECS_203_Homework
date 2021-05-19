/** @file       operatorReload.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/19/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>

class Person
{
public:
	Person() {}

	Person(int a, int b) : m_a(a), m_b(b) {}

	//	// 返回值类型 operator运算符名(参数列表) {函数体}
	//	Person operator+(Person &person) const
	//	{
	//		Person tmp;
	//		tmp.m_a = m_a + person.m_a;
	//		tmp.m_b = m_b + person.m_b;
	//		return tmp;
	//	}
	//
	//	Person &operator+(int num)
	//	{
	//		m_a += num;
	//		return *this;
	//	}

public:
	int m_a;
	int m_b;
};

Person operator<<(Person &person1, Person &person2)
{
	Person temp;
	temp.m_a = person1.m_a + person2.m_a;
	temp.m_b = person1.m_b + person2.m_b;
	return temp;
}

int main()
{
	Person person1(1, 2), person2(3, 4);
	// Person person3 = person1 + person2;
	Person person3 = operator<<(person1, person2);
	// Person person3 = person1.operator+(person2);

	std::cout << person3.m_a << " " << person3.m_b << std::endl;

	// person3 = person3 + 5;
	// person3.operator+(5);
	// std::cout << person3.m_a << " " << person3.m_b << std::endl;

	return 0;
}

