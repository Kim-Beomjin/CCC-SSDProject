#ifdef _DEBUG
#include "gmock/gmock.h"
#else

#endif

int main() {
#ifdef _DEBUG
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}