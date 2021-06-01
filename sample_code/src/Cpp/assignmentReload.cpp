/** @file       assignmentReload.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/23/2021
 *  @brief      
 *  @details    operator=
 *  @note       
 */
#include <iostream>

class Person
{
public:
	explicit Person(int age) { this->m_age = new int(age); }

	Person &operator=(const Person &person1)
	{
		if (this == &person1) { return *this; }
		if (this->m_age)
		{
			delete this->m_age;
			this->m_age = nullptr;
		}
		std::cout << (void *) person1.m_age << std::endl;
		this->m_age = new int(*person1.m_age);
		std::cout << (void *) this->m_age << std::endl;
		return *this;
	}

	~Person() { delete this->m_age; }

public:
	int *m_age;
};

int main()
{
	Person person1(18), person2(20);
	person2 = person1;
	person2 = person2;


	return 0;
}

