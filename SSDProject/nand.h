#pragma once

class NandInterface
{
public:
  virtual bool Read(const int lba, int& out) = 0;
  virtual bool Write(const int lba, const int data) = 0;
};

class Nand : public NandInterface
{
public:
  Nand() {}
  bool Read(const int lba, int& out);
  bool Write(const int lba, const int data);
private:
  bool _IsValidParameter(const int lba);
  const int LBA_START_ADDR = 0;
  const int LBA_END_ADDR = 100;
};