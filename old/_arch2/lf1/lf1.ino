int REN = 11;
int R1 = 12; // 1/0 - forward
int R2 = 13;

int LEN = 3;
int L1 = 1; // 1/0 - forward
int L2 = 2; 

void setup() {
  for( int i = 0; i<=13; i++ ) {
    pinMode( i, OUTPUT );
    digitalWrite( i, LOW );
  }  
}

void loop() {

  delay( 1000 );

  digitalWrite( R1, HIGH );
  digitalWrite( R2, LOW );
  analogWrite( REN, 255 );

  digitalWrite( L1, HIGH );
  digitalWrite( L2, LOW );
  analogWrite( LEN, 255 );

  delay( 2000 );
  
  analogWrite( REN, 0 );
  analogWrite( LEN, 0 );
  delay( 3000 );
}


