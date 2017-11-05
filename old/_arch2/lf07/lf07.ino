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
long sComp[ SCNT*2 ];

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

// parametry do normalizacji [%]
//long sMin[ SCNT ] = {  2698,  2476,  2471,  2555 };
//long sMax[ SCNT ] = { 76399, 58143, 56300, 77389 };
long sMin[ SCNT ] = {  1800,  1700,  1800,  1600 };
long sMax[ SCNT ] = { 67900, 54900, 59100, 57300 };
long sPct[ SCNT ];

	


void normalize() {
// sRaw -> sPct (0..10000)

  for( int i=0; i<SCNT; i++ ){
    long s = sRaw[ i ];
    s = s * 100;
    s = s - sMin[ i ];
    if( s<0 ) {
      s = 0;
    }
    s = s * 10000;
    s = s / ( sMax[i] - sMin[i] );
    if( s > 10000 ) {
      s = 10000;
    }
    sPct[i] = s;
  }
}




void calibrate() {
  long mn[ SCNT ];
  long mx[ SCNT ];
  readSensors();
  for( int s=0; s<SCNT; s++ ) {
    mn[s] = sRaw[s];
    mx[s] = sRaw[s];
  }
  for(;;) {
    readSensors();
    for( int s=0; s<SCNT; s++ ) {
      if( sRaw[s] > mx[s] ) {
        mx[s] = sRaw[s];
        lcd.setCursor( s*4, 1 );
        lcd.print( sRaw[s] );
        lcd.print( " ");
      } else if( sRaw[s] < mn[s] ) {
        mn[s] = sRaw[s];
        lcd.setCursor( s*4, 0 );
        lcd.print( sRaw[s] );
        lcd.print( " ");
      }
    }

//    
//    lcd.setCursor( 0, 0 );
//    lcd.print( mn[0] );
//    lcd.print( " " );
//    lcd.setCursor( 0, 1 );
//    lcd.print( mx[0] );
//    lcd.print( "  " );
    
  }
}

void loop3() {
  calibrate();
}

long computePosition() {
  int az[ 2*SCNT ];
  for( int c=0; c<2*SCNT; c++ ) {
    az[c] = sComp[c] > 0;
  }
  if( az[0] && !az[2] )
    return sComp[ 0 ]; 
  if( az[1] && az[2] )
    return (sComp[1] + sComp[2]) / 2;
  if( !az[1] && az[3] && !az[4])
    return sComp[3];
  if( az[3] && az[4] )
    return (sComp[3] + sComp[4]) / 2;
  if( !az[3] && az[4] && !az[6] )
    return sComp[4];
  if( az[5] && az[6] )
    return (sComp[5] + sComp[6]) / 2;
  if( !az[5] && az[7] )
    return sComp[7];
  return -1;  
}

const int PCNT = 16;
const long tPct[PCNT] = { 200, 300, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 9500, 9800, 9900, 10000 }; 

const long tPos[2*SCNT][PCNT] = {{ 0, 0, 0, 10, 1041, 2014, 3074, 4285, 5659, 7213, 9049, 11513, 13317, 15017, 15832, 17600 },
  { 33491, 32793, 32264, 31537, 30525, 29637, 28746, 27821, 26824, 25689, 24323, 22478, 21175, 19890, 19229, 17600 },
  { 17090, 18889, 19985, 20915, 21842, 22545, 23264, 24032, 24916, 25956, 27259, 29031, 30380, 31610, 32280, 33700 },
  { 50261, 49094, 48491, 47660, 46740, 46006, 45230, 44400, 43466, 42415, 41072, 39159, 37679, 36269, 35563, 33700 },
  { 53071, 55261, 56767, 57735, 58823, 59603, 60372, 61200, 62100, 63139, 64390, 66139, 67440, 68620, 69200, 70700 },
  { 84358, 82890, 82137, 81391, 80568, 79919, 79242, 78512, 77716, 76801, 75730, 74309, 73261, 72315, 71786, 70700 },
  { 68201, 69029, 69639, 70342, 71213, 71926, 72611, 73351, 74169, 75119, 76297, 77952, 79228, 80509, 81159, 83100 },
  { 100000, 100000, 100000, 99995, 99117, 98257, 97340, 96327, 95185, 93826, 92047, 89424, 87438, 85751, 84940, 83100 }};															




void loop() {
  readSensors();
  normalize();

  for( int s=0; s<SCNT; s++ ) {
    
    int rg;
    long sp = sPct[ s ];
//    Serial.print( sp );  
    Serial.print( " " );
  
    for( rg = 0; rg < PCNT; rg++ ) {
      if( sp < tPct[rg] ) {
        break;
      }
    }
    if( rg == PCNT ) {
      rg = PCNT - 1;
    }
  
//    Serial.print( tPct[ rg ]);
    Serial.print( " " );
  
    for( int b=0; b<=1; b++ ) {
      
      long r;
      if( rg == 0 ) {
        r = -1; // brak wyniku z danej gałęzi; może jeszcze inne warunki braku wyniku; jeszcze warunki na wynik niskiej jakości 
      } else {
        long d1 = sp - tPct[ rg-1 ];
        long d2 = tPct[ rg ] - sp;
        r = (d1*tPos[ 2*s+b ][ rg ] + d2*tPos[ 2*s+b ][ rg-1 ]) / (d1+d2);
      }
      sComp[ 2*s+b ] = r;
      Serial.print( r );
      Serial.print( " " );
    }

//    Serial.print( " /  " );
  }

  Serial.print( computePosition());
  Serial.println();  
//  delay( 50 );
}

