/** @file       class_of_class.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/12/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <utility>

class Phone
{
public:
	explicit Phone(const char *number) : m_number(number) {}

public:
	std::string m_number = {"12345"};
};

class Person
{
public:
	Person(int age, const char *name, Phone phone) : m_age(age), m_name(name), m_phone(std::move(phone)) {}

public:
	int m_age;
	std::string m_name;
	Phone m_phone;
};

int main()
{
	// Person person(25, "Jack", Phone{});
	Person person(25, "Jack", Phone("123"));
	//Phone phone = "123"; // 隐式转换
	Phone phone("123"); // 显示构造
	Phone phone1("234");


	return 0;
}
