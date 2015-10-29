#include <iostream>
 
 
void print_hello_cpp(void)
{
	std::cerr << "A cpp example" << std::endl;
}
 
 
extern "C" {
 
int SHARED_OBJECT_SUB(int arg1, int arg2)
{
	print_hello_cpp();
 
	return arg1 - arg2;
}
 
}
