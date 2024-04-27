#include <string>
#include "pico/stdlib.h"
#include "SC1602BSLB/SC1602BSLB.h"

int main()
{
    stdio_init_all();
    const LCDInterface &lcd = SC1602BSLB_8bit();

    const std::string message = "abcdefghijklmnopqrstuvwxyz0123456789";
    while (1)
    {
        lcd.Display(message);
        sleep_ms(1000);
        lcd.Clear();
        sleep_ms(1000);
    }
}