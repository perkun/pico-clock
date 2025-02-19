#include "ds1302.h"
#include "pico/stdlib.h"
#include "ssd1306/SSD1306_OLED.hpp"
#include <string>

// Screen settings
#define myOLEDwidth 128
#define myOLEDheight 64
#define myScreenSize (myOLEDwidth * (myOLEDheight / 8))  // eg 1024 bytes = 128 * 64/8
uint8_t screenBuffer[myScreenSize];

// instantiate  an OLED object
SSD1306 myOLED(myOLEDwidth, myOLEDheight);

// I2C settings
const uint16_t I2C_Speed = 100;
const uint8_t I2C_GPIO_CLK = 15;
const uint8_t I2C_GPIO_DATA = 14;

#define PIN_RST 16
#define PIN_DATA 17
#define PIN_CLK 18

// =============== Function prototype ================
void SetupScreen(void);
void UpdateScreen(const char *);
void EndScreen(void);

int main()
{
    DS1302::Clock clock(PIN_RST, PIN_CLK, PIN_DATA);

    SetupScreen();
    while (true) {
        DS1302::DateTime t = clock.getDateTime();
        UpdateScreen(t.getString().c_str());
        sleep_ms(543);
    }
    EndScreen();
}

// ===================== Function Space =====================
void SetupScreen()
{
    stdio_init_all();  // Initialize chosen serial port, default 38400 baud
    busy_wait_ms(500);
    printf("OLED SSD1306 :: Start!\r\n");
    while (myOLED.OLEDbegin(SSD1306::SSD1306_ADDR, i2c1, I2C_Speed, I2C_GPIO_DATA, I2C_GPIO_CLK) !=
           DisplayRet::Success) {
        printf("SetupTest ERROR : Failed to initialize OLED!\r\n");
        busy_wait_ms(1500);
    }  // initialize the OLED
    if (myOLED.OLEDSetBufferPtr(myOLEDwidth, myOLEDheight, screenBuffer) != DisplayRet::Success) {
        printf("SetupTest : ERROR : OLEDSetBufferPtr Failed!\r\n");
        while (1) {
            busy_wait_ms(1000);
        }
    }  // Initialize the buffer
    myOLED.OLEDFillScreen(0xF0, 0);  // splash screen bars
    busy_wait_ms(1000);
}

void UpdateScreen(const char *str)
{
    myOLED.OLEDclearBuffer();
    myOLED.setFont(pFontDefault);
    myOLED.setCursor(5, 5);
    myOLED.print(str);
    myOLED.OLEDupdate();
}

void EndScreen()
{
    myOLED.OLEDPowerDown();  // Switch off display
    myOLED.OLEDdeI2CInit();
    printf("OLED SSD1306 :: End\r\n");
}
// ============== EOF =========
