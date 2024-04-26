#include "pico/stdlib.h"

class SC1602BSLB
{
public:
    SC1602BSLB()
    {
        init();
    }

    void Display(uint8_t data)
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
        lcd_command(0x38);
        lcd_command(0x0C);
        lcd_command(0x01);
        sleep_ms(2);
        lcd_command(0x06);
    }

    void pulse_enable()
    {
        gpio_put(ENABLE, 0);
        sleep_us(1);
        gpio_put(ENABLE, 1);
        sleep_us(1);
        gpio_put(ENABLE, 0);
        sleep_us(100);
    }

    void send_byte(uint8_t byte)
    {
        gpio_put_masked(0xFF << D0, byte << D0);
        pulse_enable();
    }

    void lcd_command(uint8_t command)
    {
        gpio_put(REGISTER_SELECT, 0);
        send_byte(command);
    }

    // 書き込み専用にするためR/WはGNDへ
    const uint REGISTER_SELECT = 28;
    const uint ENABLE = 26;
    const uint D0 = 0;
    const uint D1 = 1;
    const uint D2 = 2;
    const uint D3 = 3;
    const uint D4 = 4;
    const uint D5 = 5;
    const uint D6 = 6;
    const uint D7 = 7;
};

int main()
{
    stdio_init_all();
    SC1602BSLB lcd;

    const char *text = "Hello, World!";
    while (*text)
    {
        lcd.Display(*text++);
    }

    while (1)
    {
        tight_loop_contents();
    }
}