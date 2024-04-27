#pragma once
#include "pico/stdlib.h"
#include "LCD/LCD.h"
#include <cstdint>

class SC1602BSLB_8bit : public LCDInterface
{
public:
    SC1602BSLB_8bit()
    {
        init();
    }

    void Display(uint8_t data) const
    {
        gpio_put(REGISTER_SELECT, 1);
        send_byte(data);
    }

private:
    void init()
    {
        gpio_init_mask((1UL << REGISTER_SELECT) | (1UL << ENABLE) | (0xFF << D0));
        gpio_set_dir_out_masked((1UL << REGISTER_SELECT) | (1UL << ENABLE) | (0xFF << D0));
        sleep_ms(15);
        lcd_command(FUNCTION_SET);
        lcd_command(DISPLAY_ON);
        lcd_command(CLEAR_DISPLAY);
        sleep_ms(2);
        lcd_command(ENTRY_MODE_SET);
    }

    void send_enable_signal() const
    {
        gpio_put(ENABLE, 0);
        sleep_us(1);
        gpio_put(ENABLE, 1);
        sleep_us(1);
        gpio_put(ENABLE, 0);
        sleep_us(100);
    }

    void send_byte(uint8_t byte) const
    {
        gpio_put_masked(0xFF << D0, byte << D0);
        send_enable_signal();
    }

    void lcd_command(uint8_t command)
    {
        gpio_put(REGISTER_SELECT, 0);
        send_byte(command);
    }

    // 書き込み専用にするためR/WはGNDへ
    const uint8_t REGISTER_SELECT = 28;
    const uint8_t ENABLE = 26;
    const uint8_t D0 = 0;
    const uint8_t D1 = 1;
    const uint8_t D2 = 2;
    const uint8_t D3 = 3;
    const uint8_t D4 = 4;
    const uint8_t D5 = 5;
    const uint8_t D6 = 6;
    const uint8_t D7 = 7;

private:
    const uint8_t CLEAR_DISPLAY = 0x01;
    const uint8_t FUNCTION_SET = 0x38;   // 8bit mode,2lines,10dots
    const uint8_t DISPLAY_ON = 0x0C;     // cursor,cursor position OFF
    const uint8_t ENTRY_MODE_SET = 0x06; // increment, with display shift
};
