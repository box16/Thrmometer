#pragma once
#include <cstdint>

class ThermometerIF
{
public:
    virtual float Get() const = 0;
};