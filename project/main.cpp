#include <string>
#include "pico/stdlib.h"
#include "LCD/SC1602BSLB/SC1602BSLB.h"
#include "Thermometer/10TP583T/10TP583T.h"

const uint8_t LCD_REGISTER_SELECT = 14;
const uint8_t LCD_ENABLE = 15;
const uint8_t LCD_DATA0 = 0;

const uint8_t THERMISTOR_ADC = 0; // PIN26
const uint8_t THERMOPILE_ADC = 1; // PIN27

const float THERMISTOR_SERIES_R = 100000;
const float MULTIPLIER_APPLIED_TERMOPILE = 101;
const float BIAS_ON_TERMOPILE = 16.2;

int main()
{
    stdio_init_all();

    const LCDInterface &lcd = SC1602BSLB_8bit(LCD_REGISTER_SELECT,
                                              LCD_ENABLE,
                                              LCD_DATA0);
    const ThermometerIF &thermometer = T_10TP583T(THERMISTOR_ADC,
                                                  THERMOPILE_ADC,
                                                  THERMISTOR_SERIES_R,
                                                  MULTIPLIER_APPLIED_TERMOPILE,
                                                  BIAS_ON_TERMOPILE);

    while (1)
    {
        lcd.Display("Temp: " + std::to_string(thermometer.Get()));
        sleep_ms(1000);
        lcd.Clear();
        sleep_ms(1000);
    }
}