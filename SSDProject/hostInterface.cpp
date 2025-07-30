#include "hostInterface.h"
#include <stdexcept>

void HostInterface::Execute(int argc, char* argv[])
{
	LBA lba = 0;
	DATA data = 0;
	if (argc == 4 && argv[1] == "W")
	{
		DEBUG_ASSERT(argv[2][0] != '-', "LBA CANNOT BE NEGATIVE");
		if (strlen(argv[3]) != 10)
		{
			DEBUG_ASSERT(false, "INVALID_WRITE_DATA_SIZE");
			return;
		}
		
		try {
			size_t idx;
			lba = std::stoul(argv[2], &idx, 0);
			if (idx != strlen(argv[2])) {
				throw std::invalid_argument("Invalid characters found in input");
			}
			
			data = std::stoul(argv[3], &idx, 0);
			if (idx != strlen(argv[3])) {
				throw std::invalid_argument("Invalid characters found in input");
			}
		}
		catch(std::exception)
		{
			DEBUG_ASSERT(false, "INVALID INPUT TYPE");
			return;
		}
		ssd->write(lba, data);
		
	}
	else if (argc == 3 && argv[1] == "R")
	{
		DEBUG_ASSERT(argv[2][0] != '-', "LBA CANNOT BE NEGATIVE");
		try {
			size_t idx;
			lba = std::stoul(argv[2], &idx, 0);
			if (idx != strlen(argv[2])) {
				throw std::invalid_argument("Invalid characters found in input");
			}
		}
		catch (std::exception)
		{
			DEBUG_ASSERT(false, "INVALID INPUT TYPE");
			return;
		}
		ssd->read(lba);
	}
	else {
		DEBUG_ASSERT(false, "INVALID INPUT TYPE");
	}
}