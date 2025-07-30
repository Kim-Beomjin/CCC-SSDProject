#ifdef _DEBUG
#include "gmock/gmock.h"
#else

#endif

#include "shell.h"

int main() {
#ifdef _DEBUG
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	class Shell shell;

	shell.RunShellLoop();

	return 0;
#endif
}