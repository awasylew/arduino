void setup() {
  pinMode( A5, INPUT );
  pinMode( 13, OUTPUT );
  Serial.begin( 9600 );
}

int c = 0;
int s1[10] = { 0,0,0,0,0,0,0,0,0,0 };
int s2[10] = { 0,0,0,0,0,0,0,0,0,0 };

void loop() {
//  for( int d = 0; d < 10; d++ ) {
  int d = 5;
  digitalWrite( 13, LOW );
  delay( d );
  int a1 = analogRead( A5 );
  s1[d] = (s1[d]*9 + a1) / 10;
  digitalWrite( 13, HIGH );
  delay( d );
  int a2 = analogRead( A5 );
  s2[d] = (s2[d]*9 + a2) / 10;
  Serial.print( s1[d] );
  Serial.print( " " );
  Serial.print( s2[d] );
  Serial.print( " " );
  Serial.print( s1[d] - s2[d] );
  Serial.println();
  delay( 100 );
}

