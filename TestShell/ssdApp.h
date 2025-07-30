#pragma once
#include "global_config.h"
#include "interface.h"

class SsdApp : public ISsdApp
{
public:
	DATA Read(LBA lba) override;

	bool Write(LBA lba, DATA data);

	static SsdApp* getInstance();

private:
	SsdApp() {}
};
