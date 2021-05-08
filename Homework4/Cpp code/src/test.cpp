/** @file       test.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/08/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>

int main()
{
	// a = 00000111, b = 00000100
	int a = 7, b = 4;
	int c = -1; // c = 11111111 补码

	// 00000001 => 11111110 => 11111111
	// bit-wise & | ^
	int d = -2; // 00000010 11111101 11111110


	std::cout << (a & c) << std::endl;

	return 0;
}
