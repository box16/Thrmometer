#pragma once
#include <cstdint>

class LCDInterface
{
public:
    virtual void Display(uint8_t data) const = 0;
};
