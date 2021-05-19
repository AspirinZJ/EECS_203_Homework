/** @file       leftMoveOperatorReload.cpp
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
	friend std::ostream &operator<<(std::ostream &stream, Person &person);
public:
	Person(int a, int b) : m_a(a), m_b(b) {}

private:
	int m_a;
	int m_b;
};


std::ostream &operator<<(std::ostream &stream, Person &person)
{
	stream << person.m_a << " " << person.m_b;
	return stream;
}

int main()
{
	Person person(1, 2);
	Person person2(3, 4);
	//operator<<(std::cout, person);
	std::cout << person << " "<< person2; // 链式编程思想
	// operator<<(operator<<(std::cout, person), person2);


	return 0;
}

