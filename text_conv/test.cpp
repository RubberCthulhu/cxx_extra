
#include <iostream>
#include "text_conv.h"

int main(int argc, char *argv[])
{
	TextConv *tc = new TextConv("cp1251", "utf-8");
	std::cout << "Everything is ok" << std::endl;
	return 0;
}



