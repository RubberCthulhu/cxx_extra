
#include <iostream>
#include "time_helper.h"

int main(int argc, char *argv[])
{
	time_t t = time_local_next_day();
	std::cout << "Everything is ok" << std::endl;
	return 0;
}



