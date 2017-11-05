// AW: cyklicznie przez 4 przekaźniki

#include <OneWire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd( 0x27, 20, 4 );

void setup()
{      
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  pinMode( 4, OUTPUT );
  pinMode( 5, OUTPUT );
  pinMode( 6, OUTPUT );
  pinMode( 7, OUTPUT );
}

void loop() {
  for( int i=0; i<4; i++ ) {

    lcd.clear();
    lcd.setCursor( 0, 0 );
    lcd.print( i+1 );

    digitalWrite( i+4, LOW );
    delay( 1000 );

    lcd.print( "-" );
    
    digitalWrite( i+4, HIGH );
    delay( 1000 );
    
  }
}


