/** @file       sequence_of_constructor_and_destructor.cpp
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
	Base() { std::cout << "Base constructor.\n";}
	~Base() { std::cout << "Base destructor.\n";}
};

class Derived: public Base
{
public:
	Derived() { std::cout << "Derived constructor.\n";}
	~Derived() { std::cout << "Derived destructor.\n";}
};

int main()
{
	Derived derived;

	return 0;
}

