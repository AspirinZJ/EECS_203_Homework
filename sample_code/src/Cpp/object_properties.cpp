/** @file       object_properties.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/01/2021
 *  @brief      构造函数和析构函数
 *  @details    构造函数和类型一致， 没有返回值， 不能写void
 *  @note       
 */
#include <iostream>

class Person
{
public:
	Person()
	{
		mName = "NULL";
		mAge = 22;
	}

	//	Person(std::string name, int age)
	//	{
	//		mName = name;
	//		mAge = age;
	//	}
	// 初始化列表
	Person(std::string name, int age) : mName(name), mAge(age) {}

	explicit Person(std::string name) : mName(name) {}

	std::string mName;
	int mAge = 20;
};

int main()
{
	Person person("Jack", 25);
	std::cout << person.mName << '\n' << person.mAge << std::endl;
	// Person person2(); 有空构造函数时不能写（）
	//	Person person2;
	//	std::cout << person2.mName << '\n' << person2.mAge << std::endl;

	Person person3("David");
	// Person person3 = std::string("David");
	std::cout << person3.mName << '\n' << person3.mAge << std::endl;


	return 0;
}