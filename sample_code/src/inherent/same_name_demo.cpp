/** @file       same_name_demo.cpp
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
	Base() { this->m_a = 1;}
	void func() { std::cout << "func" << std::endl;}
	int m_a;
};

class Derived : public Base
{
public:
	Derived() {this->m_a = 2; this->Base::m_a = 1;}
	void func() { std::cout << "derived func" << std::endl;}
	int m_a;
};

int main()
{
	Derived derived;
	std::cout << derived.m_a << " " << derived.Base::m_a << std::endl;

	derived.func();
	derived.Base::func();


	return 0;
}

