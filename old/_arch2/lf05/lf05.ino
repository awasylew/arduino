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

//#define MID 666
//int sMin[ SCNT ] = { MID, MID, MID, MID };
//int sMax[ SCNT ] = { MID, MID, MID, MID };

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

byte vrt1[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte vrt2[8] = {
  B01000,
  B01000,
  B01000,
  B01000,
  B01000,
  B01000,
  B01000,
  B01000
};

byte vrt3[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

byte vrt4[8] = {
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
  B00010
};

byte vrt5[8] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001
};

byte blk1[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte blk2[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte blk3[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte blk4[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};

byte blk5[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

void bar( int i ) {
    int p = i / 5;
    int c = i % 5;
    for( int f=0; f<p; f++ ) {
      lcd.setCursor( f, 0 );
      lcd.write( 14 );
    }
    lcd.setCursor( p, 0 );
    lcd.write( c+10 );
    for( int e=p+1; e<=15; e++ ) {
      lcd.setCursor( e, 0 );
      lcd.write( 32 );
    }
    lcd.setCursor( 0, 1 );
    lcd.print( i );
    lcd.print( "   " );
}

void loop2() {
  lcd.createChar( 0, vrt1 );
  lcd.createChar( 1, vrt2 );
  lcd.createChar( 2, vrt3 );
  lcd.createChar( 3, vrt4 );
  lcd.createChar( 4, vrt5 );
  lcd.createChar( 10, blk1 );
  lcd.createChar( 11, blk2 );
  lcd.createChar( 12, blk3 );
  lcd.createChar( 13, blk4 );
  lcd.createChar( 14, blk5 );
  for( int i = 0; i <= 79; i+=5 ) {
    bar( i );
  }
  bar( 79 );
  delay( 200 );
  for( int i = 79; i >= 0; i-=5 ) {
    bar( i );
  }
  bar( 0 );
  delay( 200 );
}

long i = 1;

void loop() {
  Serial.print( i );
  Serial.print( " " );
  Serial.print( i-1 );
  Serial.print( " " );
  Serial.print( i/2 );
  Serial.println();  
  delay( 1000 );
  i = i * 2;
  
}



