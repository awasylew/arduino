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

#define SENSOR_CNT 4
#define BUTTONS_PIN A0
#define DIODS_PIN A1
int SENSOR_PIN[ SENSOR_CNT ] = { A2, A3, A4, A5 };

int sensorRaw[ SENSOR_CNT ];
int sensorLit[ SENSOR_CNT ];
int s2[ SENSOR_CNT ];

void waitForButton() {
  while( analogRead( BUTTONS_PIN ) > 800 ) {
    delay( 1 );
  }
}

void readSensors() {
  digitalWrite( DIODS_PIN, LOW );
  delay( 5 );
  for( int i=0; i<4; i++ ) {
    sensorRaw[ i ] = analogRead( SENSOR_PIN[ i ]);
  }

  digitalWrite( DIODS_PIN, HIGH );
  delay( 5 );
  for( int i=0; i<4; i++ ) {
    sensorLit[ i ] = sensorRaw[ i ] - analogRead( SENSOR_PIN[ i ]);
    s2[ i ] = (3*s2[ i ] + sensorLit[ i ]) / 4;

    lcd.setCursor( (i % 2) * 6, i >> 1 );
    lcd.print( s2[ i ]);
    lcd.print( "  " );
  }
}

void message( char* msg ) {
  
//  lcd.setCursor( 0, 0 );
  lcd.clear();
  lcd.print( msg );
//  lcd.print( "   " );

  Serial.print( "message: " );
  Serial.println( msg );
}

void setup() {

  pinMode( A0, INPUT );  // przyciski
  pinMode( A1, OUTPUT ); // diody
  pinMode( A2, INPUT );
  pinMode( A3, INPUT );
  pinMode( A4, INPUT );
  
  pinMode( REN, OUTPUT );
  pinMode( R1, OUTPUT );
  pinMode( R2, OUTPUT );
  analogWrite( REN, 0 );
  digitalWrite( R1, LOW );
  digitalWrite( R2, LOW );
  pinMode( LEN, OUTPUT );
  pinMode( L1, OUTPUT );
  pinMode( L2, OUTPUT );
  analogWrite( REN, 0 );
  digitalWrite( L1, LOW );
  digitalWrite( L2, LOW );
  
  lcd.begin(16, 2);
  Serial.begin( 9600 );

  message( "press!" );
  waitForButton();
  message( "go..." );
}

void loop() {

  readSensors();
  
  int cny1 = analogRead( A3 );
  
/*
  Serial.print( cny1 );
  Serial.print( "\x9" );  
  Serial.print( analogRead( A2 ));
  Serial.print( "\x9" );  
  Serial.print( analogRead( A3 ));
  Serial.print( "\x9" );  
  Serial.print( analogRead( A4 ));
  Serial.println();
*/

//  lcd.setCursor( 0, 1 );
//  lcd.print( cny1 );
//  Serial.println( cny1 );
//  lcd.print( "   " );

  digitalWrite( R1, HIGH );
  digitalWrite( R2, LOW );

  digitalWrite( L1, HIGH );
  digitalWrite( L2, LOW );

  int MS = 170;
  if( cny1 < 500 ) { 
    analogWrite( REN, MS );
  } else {
    analogWrite( REN, 0 );
  }    
  if( cny1 > 100 ) { 
    analogWrite( LEN, MS );
  } else {
    analogWrite( LEN, 0 );
  }    
  
  
}

void loop1() {
  waitForButton();
  lcd.setCursor(0, 1);
  lcd.print( "hello!" );
  delay( 1000 );
}


