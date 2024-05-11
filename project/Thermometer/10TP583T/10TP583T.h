#pragma once
#include "pico/stdlib.h"
#include "Thermometer.h"
#include "hardware/adc.h"
#include <cstdint>
#include <cmath>

class T_10TP583T : public ThermometerIF
{
public:
    T_10TP583T(const uint8_t thermister_adc_no,
               const uint8_t thermopile_adc_no,
               const float thermister_series_r)
    {
        THERMISTOR_ADC = thermister_adc_no;
        THERMOPILE_ADC = thermopile_adc_no;
        getPinNoToADCNo();

        THERMISTOR_SERIES_R = thermister_series_r;
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

    float getThermistorTemperature()
    {
        adc_select_input(THERMISTOR_ADC);
        const float thermistor_v = (adc_read() * 3.3f / (1 << 12));
        const float thermistor_r = (THERMISTOR_SERIES_R * thermistor_v) / (3.3 - thermistor_v);
        return (THERMISTOR_B / (std::log(thermistor_r / THERMISTOR_R0) + (THERMISTOR_B / THERMISTOR_T0))) - 273.15;
    }

private:
    uint8_t THERMISTOR_ADC;
    uint8_t THERMISTOR_PIN;

    uint8_t THERMOPILE_ADC;
    uint8_t THERMOPILE_PIN;

    const float THERMISTOR_B = 3435;    // サーミスタのB定数
    const float THERMISTOR_T0 = 298.15; // サーミスタの基準温度(25度)
    const float THERMISTOR_R0 = 100000; // サーミスタの基準抵抗(25度)
    float THERMISTOR_SERIES_R;          // サーミスタとつなぐ抵抗
};