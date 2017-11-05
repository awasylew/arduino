void setup() {
  pinMode( 5, OUTPUT ); // zielony
  pinMode( 6, OUTPUT ); // czerwony
  pinMode( 7, OUTPUT ); // niebnieski
}

void loop() {
  
  int c = 100;
  
  for( int i = 0; i < 1024; i++ ) {
  
/*  digitalWrite( 5, HIGH );
  delay( 100 );
  digitalWrite( 5, LOW );
*/

/*
  digitalWrite( 6, HIGH );
  delay( c );
  digitalWrite( 6, LOW );

  digitalWrite( 7, HIGH );
  delay( c );
  digitalWrite( 7, LOW );
  */

  digitalWrite( 5, random(2) );
  digitalWrite( 6, random(2) );
  digitalWrite( 7, random(2) );
  delay( c );  
  }
  
}

