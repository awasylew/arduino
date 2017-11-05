#include <LiquidCrystal_I2C.h>

// AW: zaczynamy z nowym wyswietlaczem

LiquidCrystal_I2C lcd( 0x27, 20, 4 );

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.clear();
  lcd.print( "hello world!" );
  delay(1000);
}
