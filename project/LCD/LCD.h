#pragma once
#include <cstdint>
#include <string>

class LCDIF
{
public:
    virtual void Display(const std::string message) const = 0;
    virtual void Clear() const = 0;
};
