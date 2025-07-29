#pragma once

class Nand
{
public:
	Nand() {}
	bool read(int lba, int &out);
	bool write(int lba, int data);
};