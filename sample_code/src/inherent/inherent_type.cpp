/** @file       inherent_type.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/23/2021
 *  @brief      public, protected, private
 *  @details    public: 基类权限全部到子类不改变， protected继承： 提升到protected， private： 提升到private
 *  			基类private的内容全部无法访问
 *  @note       
 */
#include <iostream>

class Base
{
public:
	int m_public;
protected:
	int m_protected;
private:
	int m_private;
};

// public
class Public : public Base
{
public:
	void fun() {
		this->m_public = 1;
		this->m_protected = 2;
	}
};

// proteced
class Protected : protected Base
{
public:
	void fun()
	{
		this->m_public = 1;
		this->m_protected = 2;
	}
};

// private
class Private : private Base
{
public:
	void fun()
	{
		this->m_public = 1;
		this->m_protected = 2;
	}
};

int main()
{
	Public aPublic;
	aPublic.m_public = 3;

	Protected aProtected;

	return 0;
}

