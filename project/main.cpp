#include "pico/stdlib.h"
#include "SC1602BSLB/SC1602BSLB.h"

int main()
{
    stdio_init_all();
    const LCDInterface &lcd = SC1602BSLB_8bit();

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