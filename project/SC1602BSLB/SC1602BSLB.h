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

    void Display(const std::string message) const
    {
        send_message(message.substr(0, NUM_PER_LINE));

        if (message.length() > NUM_PER_LINE)
        {
            send_command(ENTER_LINE);
            send_message(message.substr(NUM_PER_LINE));
        }
    }

    void Clear() const
    {
        send_command(CLEAR_DISPLAY);
    }

private:
    void init() const
    {
        // GPIOの初期化
        gpio_init_mask((1UL << REGISTER_SELECT) | (1UL << ENABLE) | (0xFF << D0));
        gpio_set_dir_out_masked((1UL << REGISTER_SELECT) | (1UL << ENABLE) | (0xFF << D0));
        sleep_ms(15);

        // LCDの初期化
        send_command(FUNCTION_SET);
        send_command(DISPLAY_ON);
        send_command(CLEAR_DISPLAY);
        sleep_ms(2);
        send_command(ENTRY_MODE_SET);
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

    void send_message(const std::string message) const
    {
        gpio_put(REGISTER_SELECT, 1);
        for (char c : message)
        {
            send_byte(c);
        }
    }

    void send_byte(const uint8_t byte) const
    {
        gpio_put_masked(0xFF << D0, byte << D0);
        send_enable_signal();
    }

    void send_command(const uint8_t command) const
    {
        gpio_put(REGISTER_SELECT, 0);
        send_byte(command);
    }

    // 書き込み専用にするためR/WはGNDへ
    const uint8_t REGISTER_SELECT = 28;
    const uint8_t ENABLE = 26;
    // D0から8bit分を連続したPIN番号に接続する
    const uint8_t D0 = 0;
    const uint8_t NUM_PER_LINE = 16;

private:
    const uint8_t CLEAR_DISPLAY = 0x01;
    const uint8_t FUNCTION_SET = 0x38;   // 8bit mode,2lines,10dots
    const uint8_t DISPLAY_ON = 0x0C;     // cursor,cursor position OFF
    const uint8_t ENTRY_MODE_SET = 0x06; // increment, without display shift
    const uint8_t ENTER_LINE = 0xC0;     // AddressSet : 0x40
};
