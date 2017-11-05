void setup() {
  pinMode( 13, OUTPUT );
}

void loop() {
  for( int i = 0; i < 10; i++ ) {
    for( int j = 0; j < 10; j++ ) {
      digitalWrite( 13, HIGH );
      delay( i );
      digitalWrite( 13, LOW );
      delay( 10-i );
    }
  }
}

