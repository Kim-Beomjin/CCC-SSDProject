#ifdef _DEBUG
#include "gmock/gmock.h"
#else
#include "shell.h"
#endif

int main(int argc, char* argv[]) {
#ifdef _DEBUG
	testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else
	Shell shell;
	if (argc == 1) return shell.RunShellLoop();
	return shell.Runner(argc, argv);
#endif
}