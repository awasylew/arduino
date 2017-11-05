#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  pinMode( A0, OUTPUT );
  pinMode( A1, INPUT );
  pinMode( A2, INPUT );
  pinMode( A3, INPUT );
  pinMode( A4, INPUT );
  
  lcd.begin(16, 2);
  
  Serial.begin( 9600 );
}

void loop() {
  digitalWrite( A0, 1 );

  cny1 = analogRead( A1 );
  
  Serial.print( cny1 );
  Serial.print( "\x9" );  
  Serial.print( analogRead( A2 ));
  Serial.print( "\x9" );  
  Serial.print( analogRead( A3 ));
  Serial.print( "\x9" );  
  Serial.print( analogRead( A4 ));
  Serial.println();
  
  lcd.setCursor(0, 1);
  lcd.print( cny1 );
  
}

