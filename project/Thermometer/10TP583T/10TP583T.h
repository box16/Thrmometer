#pragma once
#include "pico/stdlib.h"
#include "Thermometer.h"
#include "hardware/adc.h"
#include <cstdint>

class T_10TP583T : public ThermometerIF
{
public:
    T_10TP583T(const uint8_t thermister_adc_no,
               const uint8_t thermopile_adc_no)
    {
        THERMISTOR_ADC = thermister_adc_no;
        THERMOPILE_ADC = thermopile_adc_no;
        getPinNoToADCNo();
    }

    float Get() const
    {
        return 0;
    }

private:
    // ADCは別クラス化したほうがいいかもしれない
    void getPinNoToADCNo()
    {
        switch (THERMISTOR_ADC)
        {
        case 0:
            THERMISTOR_PIN = 26;
            break;
        case 1:
            THERMISTOR_PIN = 27;
            break;
        case 2:
            THERMISTOR_PIN = 28;
            break;
        default:
            break;
        }

        switch (THERMOPILE_ADC)
        {
        case 0:
            THERMOPILE_PIN = 26;
            break;
        case 1:
            THERMOPILE_PIN = 27;
            break;
        case 2:
            THERMOPILE_PIN = 28;
            break;
        default:
            break;
        }
    }

private:
    uint8_t THERMISTOR_ADC;
    uint8_t THERMISTOR_PIN;

    uint8_t THERMOPILE_ADC;
    uint8_t THERMOPILE_PIN;
};