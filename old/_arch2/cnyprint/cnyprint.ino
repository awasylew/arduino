void setup() {
  pinMode( A0, OUTPUT );
  pinMode( A1, INPUT );
  pinMode( A2, INPUT );
  pinMode( A3, INPUT );
  pinMode( A4, INPUT );
  Serial.begin( 9600 );
}

void loop() {
  digitalWrite( A0, 1 );
  
  Serial.print( analogRead( A1 ));
  Serial.print( "\x9" );  
  Serial.print( analogRead( A2 ));
  Serial.print( "\x9" );  
  Serial.print( analogRead( A3 ));
  Serial.print( "\x9" );  
  Serial.print( analogRead( A4 ));
  Serial.println();
}

