#pragma once
#include "global_config.h"
#include "interface.h"

class Writer : public IExecutor
{
public:
	bool execute(const string& command, LBA lba, DATA data, ISsdApp* app) override;
};

class Reader : public IExecutor
{
public:
	bool execute(const string& command, LBA lba, DATA data, ISsdApp* app) override;
};

class Helper : public IExecutor
{
public:
	bool execute(const string& command, LBA lba, DATA data, ISsdApp* app) override;
};

class Exiter : public IExecutor {
public:
	bool execute(const string& command, LBA lba, DATA data, ISsdApp* app) override;
};
