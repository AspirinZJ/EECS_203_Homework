/** @file       logicOperatorReload.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/23/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <utility>

class Person
{
public:
	Person(int age, std::string name) : m_age(age), m_name(std::move(name)) {}

	bool operator==(const Person &person) const { return this->m_age == person.m_age; }

	bool operator!=(const Person &person) const { return this->m_age != person.m_age; }

public:
	int m_age;
	std::string m_name;
};

int main()
{
	Person alice(18, "Alice"), bob(18, "Bob");

	std::cout << (alice != bob) << std::endl;


	return 0;
}

