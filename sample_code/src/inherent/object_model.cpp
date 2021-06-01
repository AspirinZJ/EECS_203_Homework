/** @file       object_model.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/23/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>

class Base
{
public:
	void print() { std::cout << m_a << m_b << m_c << std::endl; }
	Base() : m_const(0) {}

	int m_a;
	static int m_static;
	const int m_const;
protected:
	int m_b;
private:
	int m_c;
};


class Derived : public Base
{
public:
	int m_d;
};

int main()
{
	Base base;
	std::cout << sizeof(base) << std::endl;
	Derived derived;
	std::cout << sizeof(derived) << std::endl;


	return 0;
}

