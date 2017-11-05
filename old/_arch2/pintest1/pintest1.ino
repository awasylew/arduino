void setup() {
  for( int i = 0; i <= 13; i++ ) {
     pinMode( i, OUTPUT );
  }
}

void loop() {
  for( int i = 0; i <= 3; i++ ) {
    digitalWrite( i, LOW );
  }
  for( int i = 8; i <= 12; i++ ) {
    digitalWrite( i, HIGH );
  }
  
  for( int i = 4; i <= 7; i++ ) {
    digitalWrite( i, HIGH );
  }
  digitalWrite( 13, HIGH );
  delay( 5000 );

  for( int i = 4; i <= 7; i++ ) {
    digitalWrite( i, LOW );
  }
  digitalWrite( 13, LOW );
  delay( 5000 );
}
