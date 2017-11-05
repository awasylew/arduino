int BUTTON1 = 2;
int BUTTON2 = 3;

int buttonState1 = 0;
int buttonState2 = 0;

unsigned long buttonMillis1 = 0;
unsigned long buttonMillis2 = 0;

unsigned long buttonDelay = 100;
int buttonCount = 0;

int LED = 13;

void setup() {
  pinMode( LED, OUTPUT );
  digitalWrite( LED, LOW ); 
  Serial.begin( 9600 );
  Serial.println( "ready..." );
//  digitalWrite( LED, HIGH ); 
  pinMode( BUTTON1, INPUT_PULLUP );
  pinMode( BUTTON2, INPUT_PULLUP );
  buttonMillis1 = millis();
  buttonMillis2 = millis();
}

void loop() {
//  Serial.print( 1 - digitalRead( BUTTON1 ));
//  Serial.print( 1 - digitalRead( BUTTON2 ));
//  Serial.println();

  int b1 = 1 - digitalRead( BUTTON1 );
  if( b1 != buttonState1  &&  millis() > buttonMillis1 + buttonDelay ) {
    buttonState1 = b1;
    buttonMillis1 = millis();
    buttonCount++;
    Serial.print( buttonCount );
    Serial.print( ":jeden" );
    Serial.print( b1 );
    Serial.print( "@" );
    Serial.print( millis());
    Serial.println();
  }

//  Serial.print( "tick" );
//  Serial.println( millis() );

  int s = Serial;
  int l = ((((int) millis()) >> 4) & 111) == 0;
//  digitalWrite( LED, ((((int) millis()) >> 4) & 111) == 0  s ); 
  Serial.println( (((int) millis()) >> 10) & 7 );
  digitalWrite( LED, (s+l) == 1 ); 
  delay( 30 );
}


