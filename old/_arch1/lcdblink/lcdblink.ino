#include <avr/sleep.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int enable = 9;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.blink();
  
}

void loop() {
  
  for( int i = 10; i >= 0; i-- ) {
    
    lcd.clear();
    lcd.print( i );
    lcd.print( "." );

//  pinMode( enable, OUTPUT );
//  digitalWrite( enable, HIGH );
    delay( 1000 );


  }

  lcd.clear();
  lcd.print( "sleep" );
  
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here
//  sleep_enable();          // enables the sleep bit in the   
//  sleep_mode();
  
  lcd.clear();
  lcd.print( "up" );
 
  for( ;; ) {
  }
  
}

