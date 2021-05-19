/** @file       const.cpp
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
	Person() : m_a(1), m_b(2) {}

	void show() const { std::cout << m_a << " " << m_b << std::endl; m_b = 1000;}
	void show2() { std::cout << m_a << " " << m_b << std::endl; }

public:
	int m_a;
	mutable int m_b;
};

int main()
{
	const Person person;
	const int arrSize = 5;
	person.show();
	// person.show2();
	return 0;
}
