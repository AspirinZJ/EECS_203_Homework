/** @file       friend.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/19/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>

class Person;

class Friend
{
public:
	Friend();
	void visitMyFriend();

public:
	Person *pPerson;
};

class Person
{
	friend void showPerson(Person *pPerson);
	// friend class Friend;
	friend void Friend::visitMyFriend();

public:
	Person() { m_name = std::string("name");}

public:
	int m_age;
private:
	std::string m_name;
};

Friend::Friend() {pPerson = new Person;}
void Friend::visitMyFriend() {std::cout << pPerson->m_age << " " << pPerson->m_name << std::endl;}

void showPerson(Person *pPerson) // global
{
	std::cout << pPerson->m_age << " " << pPerson->m_name << std::endl;
}

int main()
{
	Person person;
	Friend aFriend;
	aFriend.visitMyFriend();

	return 0;
}
