#include <EEPROM.h>

int sensePin = 3;
int modePin = 2;

int writePos = 0;

void setup() {
  Serial.begin( 9600 );
  Serial.println( "ready..." );
  
  pinMode( sensePin, INPUT_PULLUP );
  pinMode( modePin, INPUT_PULLUP );
}

void dump() {
  Serial.println( "dumping..." );
  for( int i = 0; i < 10; i++ ){
    Serial.print( "dump " );
    Serial.print( i );
    Serial.print( " = " );
    Serial.print( EEPROM.read( i ));
    Serial.println();
  }
}

void loop() {
    while(( digitalRead( sensePin ) == HIGH ) &&
        (digitalRead( modePin ) == HIGH )) {
    }
    Serial.println( "the mode is..." );
    
    if( digitalRead( modePin ) == LOW ) {
        dump();
        for(;;) {}
    }
    Serial.println( "registering..." );
    
    for(;;) {
      while( digitalRead( sensePin ) == HIGH ) {
      }
      unsigned long time = millis();
      time = time / 1000;
      EEPROM.write( writePos, (int) time );
      writePos++;    
      Serial.println( time );
      
      delay( 50 );
      while( digitalRead( sensePin ) == LOW ) {
      }
    }
}

