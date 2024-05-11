#pragma once
#include "pico/stdlib.h"
#include "Thermometer.h"
#include <cstdint>

class T_10TP583T : public ThermometerIF
{
public:
    float Get() const
    {
        return 0;
    }
};