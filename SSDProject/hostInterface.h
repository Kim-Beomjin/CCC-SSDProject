#pragma once
#include "ssd.h"

class HostInterface
{
public:
    static HostInterface* GetInstance() {
        static HostInterface hostinterface; 
        return &hostinterface;
    }

    void Execute(int argc, char* argv[]);
#if _DEBUG
    void SetSSD(SSD* ssd)
    {
        this->ssd = ssd;
    }
#endif

private:
    HostInterface() : ssd{ new SSD() } {};
    SSD* ssd;
};