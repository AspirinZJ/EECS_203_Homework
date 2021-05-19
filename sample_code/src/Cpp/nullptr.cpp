/** @file       nullptr.cpp
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
	void show() { std::cout << this->m_age << std::endl; }

	void show2()
	{
		if (this == nullptr)
		{
			std::cout << "nullptr" << std::endl;
		}
		else { std::cout << this->m_age << std::endl; }
	}

public:
	int m_age = 0;
};

int main()
{
	Person *pPerson = nullptr;

	pPerson->show2();

	return 0;
}
