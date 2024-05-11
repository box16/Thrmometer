#pragma once
#include "pico/stdlib.h"
#include "LCD.h"
#include <cstdint>

class SC1602BSLB_8bit : public LCDIF
{
public:
    SC1602BSLB_8bit(const uint8_t register_select_pin,
                    const uint8_t enable_pin,
                    const uint8_t data_0_pin // DATA0から8bit分を連続したPIN番号に接続する想定
                    /*const uint8_t read_write_pin 書き込み専用にするためGND接続する想定*/)
    {
        init(register_select_pin,
             enable_pin,
             data_0_pin);
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
    void init(const uint8_t register_select_pin,
              const uint8_t enable_pin,
              const uint8_t data_0_pin)
    {
        REGISTER_SELECT = register_select_pin;
        ENABLE = enable_pin;
        DATA0 = data_0_pin;

        // GPIOの初期化
        gpio_init_mask((1UL << REGISTER_SELECT) | (1UL << ENABLE) | (0xFF << DATA0));
        gpio_set_dir_out_masked((1UL << REGISTER_SELECT) | (1UL << ENABLE) | (0xFF << DATA0));
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
        gpio_put_masked(0xFF << DATA0, byte << DATA0);
        send_enable_signal();
    }

    void send_command(const uint8_t command) const
    {
        gpio_put(REGISTER_SELECT, 0);
        send_byte(command);
    }

private:
    uint8_t REGISTER_SELECT;
    uint8_t ENABLE;
    uint8_t DATA0;

    const uint8_t NUM_PER_LINE = 16;
    const uint8_t CLEAR_DISPLAY = 0x01;
    const uint8_t FUNCTION_SET = 0x38;   // 8bit mode,2lines,10dots
    const uint8_t DISPLAY_ON = 0x0C;     // cursor,cursor position OFF
    const uint8_t ENTRY_MODE_SET = 0x06; // increment, without display shift
    const uint8_t ENTER_LINE = 0xC0;     // AddressSet : 0x40
};
