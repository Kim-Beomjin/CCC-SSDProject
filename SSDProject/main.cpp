#ifdef _DEBUG
#include "gmock/gmock.h"
#else
#include "hostInterface.h"
#include <string>
#include <iostream>
#endif

int main(int argc, char* argv[]) {
#ifdef _DEBUG
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	HostInterface::GetInstance()->Execute(argc, argv);
#endif
}