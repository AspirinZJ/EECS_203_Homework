/** @file       autoGenerate.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/19/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <vector>

class Person
{
private:
	std::string m_name;
	int m_age;
	double m_height;
public:
	const std::string &getMName() const;
	int getMAge() const;
	double getMHeight() const;
	const std::vector<int> &getVec() const;
private:
	std::vector<int> vec;
};

int main()
{

	return 0;
}

