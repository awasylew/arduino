#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// 1/0 - forward
#define REN 11
#define R1 12
#define R2 13 

// 1/0 - forward
#define LEN 3
#define L1 2 
#define L2 1

#define SCNT 4
#define BUTTONS_PIN A0
#define DIODS_PIN A1
int SENSOR_PIN[ SCNT ] = { A2, A3, A4, A5 };

int sRaw[ SCNT ];

void waitForButton() {
  while( analogRead( BUTTONS_PIN ) > 800 ) {
    delay( 1 );
  }
}

void readSensors() {
  for( int i=0; i<4; i++ ) {
    sRaw[ i ] = analogRead( SENSOR_PIN[ i ]);
  }
}


void message( char* msg ) {
  
  lcd.clear();
  lcd.print( msg );

  Serial.print( "message: " );
  Serial.println( msg );
}

void setup() {

  pinMode( A0, INPUT );  // przyciski
  pinMode( A1, OUTPUT ); // diody
  digitalWrite( DIODS_PIN, HIGH );

  pinMode( A2, INPUT );  // sensor
  pinMode( A3, INPUT );  // sensor
  pinMode( A4, INPUT );  // sensor
  pinMode( A5, INPUT );  // sensor
  
  pinMode( REN, OUTPUT ); // motor stop
  pinMode( R1, OUTPUT );
  pinMode( R2, OUTPUT );
  analogWrite( REN, 0 );
  digitalWrite( R1, LOW );
  digitalWrite( R2, LOW );

  pinMode( LEN, OUTPUT );  // motor stop
  pinMode( L1, OUTPUT );
  pinMode( L2, OUTPUT );
  analogWrite( REN, 0 );
  digitalWrite( L1, LOW );
  digitalWrite( L2, LOW );
  
  lcd.begin(16, 2);
  Serial.begin( 115200 );

  message( "press!" );
  waitForButton();
  message( "go..." );
}

void loop1() {
  readSensors();
  Serial.print( millis());
  Serial.print( ", " );
  for( int i = 0; i < SCNT; i++ ) {
    Serial.print( sRaw[ i ] );
    Serial.print( ", " );
  }
  Serial.println();
}

long tPrc1a[ 13 ] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 95, 98, 99, 100 };
long tPrc1b[ 13 ] = { 100, 99, 98, 95, 90, 80, 70, 60, 50, 40, 30, 20, 10 };

long tPos1a[ 13 ] = { 10, 1041, 2014, 3074, 4285, 5659, 7213, 9049, 11513, 13317, 15017, 15832, 17600 };
long tPos1b[ 13 ] = { 17600, 19229, 19890, 21175, 22478, 24323, 25689, 26824, 27821, 28746, 29637, 30525, 31537 };

long tPos2a[ 13 ] = { 20915, 21842, 22545, 23264, 24032, 24916, 25956, 27259, 29031, 30380, 31610, 32280, 33700 };
long tPos2b[ 13 ] = { 33700, 35563, 36269, 37679, 39159, 41072, 42415, 43466, 44400, 45230, 46006, 46740, 47660 };


void loop() {
  readSensors();
//  Serial.print( sRaw[ 0 ] );
//  Serial.print( " " );
  long sPrc1 = (long(sRaw[ 0 ])) * 100 - 2700;  
  long sPrc2 = (long(sRaw[ 1 ])) * 100 - 2500;  
  if( sPrc1<0 ) {
    sPrc1 = 0;
  }
  sPrc1 = sPrc1 / (764-27);
  sPrc2 = sPrc2 / (582-25);
//  Serial.print( sPrc1 );  
//  Serial.print( " " );

  int i;
  for( i = 0; i < 13; i++ ) {
    if( sPrc1 < tPrc1a[ i ] ) {
      break;
    }
  }

  Serial.print( tPrc1a[ i ]);
  Serial.print( " " );

  if((i > 0) && (i < 13)) {
    Serial.print( "1a " );
    
    long d1 = sPrc1 - tPrc1a[ i-1 ];
    long d2 = tPrc1a[ i ] - sPrc1;
    long r = (d1*tPos1a[ i ] + d2*tPos1a[ i-1 ]) / (d1+d2);
    Serial.print( r );
    Serial.print( " " );
  }

  for( i = 0; i < 13; i++ ) {
    if( sPrc1 > tPrc1b[ i ] ) {
      break;
    }
  }

  Serial.print( tPrc1b[ i ]);
  Serial.print( " " );

  if((i > 0) && (i < 13)) {
    Serial.print( "1b " );
    
    long d1 = tPrc1b[ i-1 ] - sPrc1;
    long d2 = sPrc1 - tPrc1b[ i ];
    long r = (d1*tPos1b[ i ] + d2*tPos1b[ i-1 ]) / (d1+d2);
    Serial.print( r );
    Serial.print( " " );
  }

  Serial.print( tPrc1a[ i ]);
  Serial.print( " " );

  for( i = 0; i < 13; i++ ) {
    if( sPrc2 < tPrc1a[ i ] ) {
      break;
    }
  }

  if((i > 0) && (i < 13)) {
    Serial.print( "2a " );
    
    long d1 = sPrc2 - tPrc1a[ i-1 ];
    long d2 = tPrc1a[ i ] - sPrc2;
    long r = (d1*tPos2a[ i ] + d2*tPos2a[ i-1 ]) / (d1+d2);
    Serial.print( r );
    Serial.print( " " );
  }

  for( i = 0; i < 13; i++ ) {
    if( sPrc2 > tPrc1b[ i ] ) {
      break;
    }
  }

  Serial.print( tPrc1b[ i ]);
  Serial.print( " " );

  if((i > 0) && (i < 13)) {
    Serial.print( "2b " );
    
    long d1 = tPrc1b[ i-1 ] - sPrc2;
    long d2 = sPrc2 - tPrc1b[ i ];
    long r = (d1*tPos2b[ i ] + d2*tPos2b[ i-1 ]) / (d1+d2);
    Serial.print( r );
    Serial.print( " " );
  }




  Serial.println();  
  delay( 100 );
}

