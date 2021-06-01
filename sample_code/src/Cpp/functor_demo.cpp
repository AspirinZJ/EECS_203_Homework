/** @file       functor_demo.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/23/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>

class Person
{
public:
	Person(int age, std::string name) : m_age(age), m_name(std::move(name)) {}

	void operator()()
	{
		std::cout << this->m_age << " " << this->m_name << std::endl;
	}

	void operator()(int age, std::string name)
	{
		this->m_age = age;
		this->m_name = std::move(name);
	}


private:
	int m_age;
	std::string m_name;
};

int main()
{
	Person person(20, "jack");
	person();
	person(18, "alice");
	person();

	Person(30, "John")();


	return 0;
}

