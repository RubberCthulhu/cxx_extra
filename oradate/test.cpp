
#include <iostream>
#include "oradate.h"

int main(int argc, char *argv[])
{
	oracle::occi::Environment *envhp = oracle::occi::Environment::createEnvironment(
		oracle::occi::Environment::THREADED_MUTEXED);
	oracle::occi::Date D(envhp);
	time_t t = oradateToUnixtime(D);
	std::cout << "Everything is ok" << std::endl;
	return 0;
}



