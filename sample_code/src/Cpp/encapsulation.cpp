/** @file       abstract.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/01/2021
 *  @brief      面向对象： 封装、继承、多态
 *  @details    privilege: public, protected, private
 *  			public: 类内访问，类外访问
 *  			private：类内访问，类外不访问, 子类不访问
 *  			protected: 类内访问，类外不访问，子类可以访问
 *
 *  			classs 和 struct区别： struct默认public
 *  @note       
 */
#include <iostream>

class Circle
{
public:
	double radius; // 属性
	double getCircumference() // 函数
	{
		return 2 * 3.14 * radius;
	}
};

class Student
{
public:
	std::string mName;

	void showName() { std::cout << mName << std::endl; }
	void showId() {std::cout << mId << std::endl;}
private:
	int mId = 123;
};

int main()
{
	//	Circle circle;
	//	circle.radius = 1;
	//	std::cout << circle.getCircumference() << std::endl;

	Student student;

	student.mName = "Jack";
	student.showName();
	student.showId();

	return 0;
}