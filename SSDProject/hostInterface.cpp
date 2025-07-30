#include "hostInterface.h"

void HostInterface::Execute(int argc, char* argv[])
{
	ssd->write(argc, atoi(argv[0]));
}