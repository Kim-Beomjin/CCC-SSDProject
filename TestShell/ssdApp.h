#pragma once
#include "global_config.h"
#include "interface.h"

class SsdApp : public ISsdApp
{
public:
	bool Read(LBA lba) override;

	bool Write(LBA lba, DATA data) override;

	bool Erase(LBA lba, SIZE size) override;

	bool Flush() override;

	static SsdApp* getInstance();

private:
	SsdApp() {}
};
