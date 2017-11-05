#include <avr/sleep.h>

int LED = 13;
int wakePIN = 2;

void wakeUpNow() {
}

void setup() {
  pinMode( LED, OUTPUT );
  digitalWrite( LED, HIGH );

  pinMode( wakePIN, INPUT_PULLUP );
  attachInterrupt(0, wakeUpNow, HIGH);  
}

void loop() {
  
 delay( 3000 ); 
 set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
 sleep_enable();
 digitalWrite( LED, LOW );
 sleep_mode();                        // System sleeps here
 sleep_disable();    
 
 
  for(;;){
    digitalWrite( LED, HIGH );
    delay( 300 );
    digitalWrite( LED, LOW );
    delay( 300 );
  }

}


