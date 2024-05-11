#include <string>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "LCD/SC1602BSLB/SC1602BSLB.h"
#include "Thermometer/10TP583T/10TP583T.h"
#include <cmath>

const uint8_t LCD_REGISTER_SELECT = 14;
const uint8_t LCD_ENABLE = 15;
const uint8_t LCD_DATA0 = 0;

const uint8_t THERMISTOR_ADC = 0; // PIN26
const uint8_t THERMOPILE_ADC = 1; // PIN27
const uint8_t THERMISTOR_PIN = 26;
const uint8_t THERMOPILE_PIN = 27;

const float THERMISTOR_SERIES_R = 100000;

int main()
{

    stdio_init_all();

    adc_init();
    adc_gpio_init(THERMISTOR_PIN);
    adc_gpio_init(THERMOPILE_PIN);

    const LCDInterface &lcd = SC1602BSLB_8bit(LCD_REGISTER_SELECT,
                                              LCD_ENABLE,
                                              LCD_DATA0);
    const ThermometerIF &thermometer = T_10TP583T(THERMISTOR_ADC,
                                                  THERMOPILE_ADC,
                                                  THERMISTOR_SERIES_R);

    const float B = 3435;    // サーミスタのB定数
    const float T0 = 298.15; // サーミスタの基準温度(25度)
    const float R0 = 100000; // サーミスタの基準抵抗(25度)
    const float Rt = 100000; // サーミスタとつなぐ抵抗

    while (1)
    {
        adc_select_input(0);
        float thermistor_v = (adc_read() * 3.3f / (1 << 12));
        float thermistor_r = (Rt * thermistor_v) / (3.3 - thermistor_v);
        float thermistor_t = (B / (std::log(thermistor_r / R0) + (B / T0))) - 273.15;

        adc_select_input(1);
        float K = 101;
        float thermopile_v = ((adc_read() * 3.3f / (1 << 12)) / K) * 1000 - 16.2;
        float target_t = 0.7578 * thermistor_t + 6.9087 * thermopile_v;

        lcd.Display("thermistor_t: " + std::to_string(thermistor_t));
        sleep_ms(1000);
        lcd.Clear();
        sleep_ms(1000);

        lcd.Display("thermopile_v: " + std::to_string(thermopile_v));
        sleep_ms(1000);
        lcd.Clear();
        sleep_ms(1000);

        lcd.Display("target_t: " + std::to_string(target_t));
        sleep_ms(1000);
        lcd.Clear();
        sleep_ms(1000);
    }
}