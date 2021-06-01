/** @file       plusplusReload.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/23/2021
 *  @brief      reload operator++
 *  @details
 *  @note       
 */
#include <iostream>

class MyInteger
{
	friend std::ostream &operator<<(std::ostream &os, MyInteger myInteger);
public:
	MyInteger() : m_int(0) {}

	MyInteger &operator++() // 前置++运算符重载
	{
		m_int++;
		return *this;
	}

	// 后置++运算符重载
	MyInteger operator++(int)
	{
		MyInteger tmp = *this;
		this->m_int++;
		return tmp;
	}

private:
	int m_int;
};

std::ostream &operator<<(std::ostream &os, MyInteger myInteger)
{
	os << myInteger.m_int;
	return os;
}


int main()
{
	MyInteger myInteger;
	std::cout << myInteger << std::endl;

	std::cout << ++myInteger << std::endl;
	std::cout << myInteger++ << std::endl;



	return 0;
}

