/** @file       shallow_deep_copy.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/08/2021
 *  @brief      深拷贝与浅拷贝
 *  @details    浅拷贝：赋值，拷贝 深拷贝：在堆区重新申请空间拷贝
 *  			浅拷贝：堆区到内存重复释放， 解决：深拷贝
 *  @note       如果成员属性中有堆区内容，一定要自定义拷贝构造函数，防止浅拷贝
 */

#include <iostream>

class Person
{
public:
	Person() { std::cout << "default" << std::endl; }

	Person(int age, float height)
	{
		m_age = age;
		m_height = new float(height); // 在堆区开辟的
		std::cout << "parameter" << std::endl;
	}

	// 深拷贝
	Person(const Person &person)
	{
		std::cout << "deep copy constructor" << std::endl;
		m_age = person.m_age;
		m_height = new float(*person.m_height);
	}

	~Person() // 析构
	{
		if (m_height != nullptr)
		{
			delete m_height;
			m_height = nullptr;
		}
		std::cout << "destructor" << std::endl;
	}

public:
	int m_age;
	float *m_height;
};

int main()
{
	Person person(25, 180);
	Person person1(person);
	std::cout << person.m_height << std::endl << person1.m_height << std::endl;

	return 0;
}
