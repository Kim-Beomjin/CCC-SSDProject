#pragma once
#include "interface.h"
#include <vector>
// todo
class CommandBuffer : public IBufferStrategy
{
public:
	void ConvertBufferToCmd(std::vector<std::string>& CmdList) override;
	void ConvertCmdToBuffer(const std::vector<std::string> CmdList) override;
	Buffer GetBuffer(LBA lba) override;
};

class DumbBuffer : public IBufferStrategy
{
	void ConvertBufferToCmd(std::vector<std::string>& CmdList) override;
	void ConvertCmdToBuffer(const std::vector<std::string> CmdList) override;
	Buffer GetBuffer(LBA lba) override;
};