// AW: min/max, min/miax z 10, ze 100 i z 1000


#include <OneWire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd( 0x27, 20, 4 );

// DS18S20 Temperature chip i/o
OneWire ds(2);  // pin termometru

int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;

#define MAX_TERM 4
int termCnt;
byte termAddr[MAX_TERM][8];
int termTemp[MAX_TERM];     // [1/100 C]
int minTemp, maxTemp;       // [1/100 C]

void readAllThermometers(void) {
// odczyt wszystkich podłączonych termometrów
  
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];

#ifdef SERIAL_READ_TEMP
  Serial.println( "początek pętli czytania" );
#endif
  ds.reset_search();

  termCnt = 0;
  minTemp = 10000;
  maxTemp = 0;

  while(1) {
      
    
    if ( !ds.search(addr)) {
#ifdef SERIAL_READ_TEMP
        Serial.print("No more addresses.\n");
#endif
        break;
    }

#ifdef SERIAL_READ_TEMP
    Serial.print("R=");
    for( i = 0; i < 8; i++) {
      Serial.print(addr[i], HEX);
      Serial.print(" ");
    }
  
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("CRC is not valid!\n");
        continue;
    }
  
    if ( addr[0] == 0x10) {
        Serial.print("Device is a DS18S20 family device.  ");
    }
    else if ( addr[0] == 0x28) {
        Serial.print("Device is a DS18B20 family device.  ");
    }
    else {
        Serial.print("Device family is not recognized: 0x");
        Serial.println(addr[0],HEX);
        continue;
    }
#endif
  
    ds.reset();
    ds.select(addr);
    ds.write(0x44,1);         // start conversion, with parasite power on at the end
  
    delay(1000);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.
  
    present = ds.reset();
    ds.select(addr);    
    ds.write(0xBE);         // Read Scratchpad

#ifdef SERIAL_READ_TEMP
    Serial.print("P=");
    Serial.print(present,HEX);
    Serial.print(" ");
#endif

    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();

#ifdef SERIAL_READ_TEMP
      Serial.print(data[i], HEX);
      Serial.print(" ");
#endif
    }
    
#ifdef SERIAL_READ_TEMP
    Serial.print(" CRC=");
    Serial.print( OneWire::crc8( data, 8), HEX);
    Serial.print( "  " );
#endif

    if( OneWire::crc8( data, 8 ) != data[ 8 ] ) {
        Serial.print("CRC is not valid!\n");
        continue;
    }
   
    LowByte = data[0];
    HighByte = data[1];
    TReading = (HighByte << 8) + LowByte;
    SignBit = TReading & 0x8000;  // test most sig bit
    if (SignBit) // negative
    {
      TReading = (TReading ^ 0xffff) + 1; // 2's comp
    }
    Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
  
    Whole = Tc_100 / 100;  // separate off the whole and fractional portions
    Fract = Tc_100 % 100;
  
  
#ifdef SERIAL_READ_TEMP
    if (SignBit) // If its negative
    {
       Serial.print("-");
    }
    Serial.print(Whole);
    Serial.print(".");
    if (Fract < 10)
    {
       Serial.print("0");
    }
    Serial.print(Fract);
  
    Serial.print("\n");
#endif

    for( i = 0; i < 8; i++) {
      termAddr[termCnt][i] = addr[i];
    }
    termTemp[termCnt] = Tc_100;
    if( Tc_100 < minTemp )
      minTemp = Tc_100;
    if( Tc_100 > maxTemp )
      maxTemp = Tc_100;  
    termCnt++;

    if( termCnt == MAX_TERM ) {
#ifdef SERIAL_READ_TEMP
      Serial.println( "MAX_TERM reached!" );
#endif
      break;
    }  

  }

#ifdef SERIAL_READ_TEMP
  Serial.print("koniec pętli czytania, termCnt=");
  Serial.print( termCnt );
  Serial.println();
#endif

}

/*
void serialAllThermometers() {
  for( int i=0; i<termCnt; i++ ) {
    for( int a=0; a<8; a++ ) {
      Serial.print( termAddr[i][a], HEX );
      Serial.print( " " );
    }
    Serial.print( "   " );
    Serial.print( termTemp[i] );
    Serial.println();
  }
}

void lcdAllThermometers() {
// prezentuje odczyty temperatury i skrajnosci
  
  lcd.clear();
  
  lcd.setCursor(0,0);
  for( int i=0; i<termCnt; i++ ) {
    lcd.print( termTemp[i] / 10 );
    lcd.print( " " );
  }

  lcd.setCursor(0,1);
  lcd.print( "min=" );
  lcd.print( minTemp/10 );
  lcd.print( "  max=" );
  lcd.print( maxTemp/10 );
}

#define MAX_CYCLE 4
int cyclicMin[MAX_CYCLE];
int cyclicMax[MAX_CYCLE];

int cycle = 0;

void cycleTemp() {
  cyclicMin[cycle] = minTemp;  
  cyclicMax[cycle] = maxTemp;  

  cycle++;
  if( cycle >= MAX_CYCLE )
    cycle = 0;
}

void cyclicUpdate() {
  if( minTemp < cyclicMin[cycle])
      cyclicMin[cycle] = minTemp;
  if( maxTemp > cyclicMax[cycle])
      cyclicMax[cycle] = maxTemp;
}

void cycleTick() {
  cycle++;
  if( cycle >= MAX_CYCLE )
    cycle = 0;
  cyclicMin[cycle] = 10000;
  cyclicMax[cycle] = 0;
}

void lcdCycles() {
  lcd.clear();

  lcd.setCursor(0,0);
  for( int i=0; i<MAX_CYCLE; i++ ) {
    lcd.print( cyclicMin[i]/10 );
    if( i == cycle )
      lcd.print( "<" );
    else
      lcd.print( " " );
  }
  
  lcd.setCursor(0,1);
  for( int i=0; i<MAX_CYCLE; i++ ) {
    lcd.print( cyclicMax[i]/10 );
    if( i == cycle )
      lcd.print( "<" );
    else
      lcd.print( " " );
  }
}

int macroC = 0;
*/

#define DIV 10
int minT1[DIV], maxT1[DIV];

int c1 = 0;

void cycle1() {
  minT1[c1] = minTemp;
  maxT1[c1] = maxTemp;
  c1++;
  if( c1 >= DIV ) {
    c1 = 0;
  }
}

int cMin, cMax;

void calc1( int* minT, int* maxT ) {
  cMin = minT[0];
  cMax = maxT[0];
  for( int i = 1; i<DIV; i++ ) {
    if( minT[i] < cMin )
      cMin = minT[i];
    if( maxT[i] > cMax )
      cMax = maxT[i]; 
  }

  for( int i=0; i<DIV; i++ ){
    Serial.print( maxT[i] );
    Serial.print( ' ' );
  }
  Serial.println();
  
}

int minT2[DIV], maxT2[DIV];

int c2 = 0;

void cycle2() {
  minT2[c2] = cMin;
  maxT2[c2] = cMax;
  c2++;
  if( c2 >= DIV ) {
    c2 = 0;
  }
}

int minT3[DIV], maxT3[DIV];

int c3 = 0;

void cycle3() {
  minT3[c3] = cMin;
  maxT3[c3] = cMax;
  c3++;
  if( c3 >= DIV ) {
    c3 = 0;
  }
}


#define RELAY 2

int cMin2, cMax2, cMin3, cMax3;

void setup()
{      
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  readAllThermometers();
  for( int i=0; i<DIV; i++ ) {
    minT1[i] = minTemp;
    minT2[i] = minTemp;
    minT3[i] = minTemp;
    maxT1[i] = maxTemp;
    maxT2[i] = maxTemp;
    maxT3[i] = maxTemp;
  }

  cMin2 = minTemp;
  cMax2 = maxTemp;
  cMin3 = minTemp;
  cMax3 = maxTemp;
  
}



void loop() {
  lcd.setCursor( 19, 0 );
  lcd.print( '*' );
  readAllThermometers();
  lcd.setCursor( 19, 0 );
  lcd.print( ' ' );

  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print( minTemp );
  lcd.print( ' ' );
  lcd.print( maxTemp );
  Serial.print( minTemp );
  Serial.print( ' ' );
  Serial.println( maxTemp );

  cycle1();
  calc1( minT1, maxT1 );

  if( c1 == 0 ) {
    cycle2();
    calc1( minT2, maxT2 );
    cMin2 = cMin;
    cMax2 = cMax;

    if( c2 == 0 ) {
      cycle3();
      calc1( minT3, maxT3 );
      cMin3 = cMin;
      cMax3 = cMax;
    }
  }

  lcd.setCursor( 0, 1 );
  lcd.print( cMin );
  lcd.print( ' ' );
  lcd.print( cMax );

  lcd.setCursor(0,2);
  lcd.print( cMin2 );
  lcd.print( ' ' );
  lcd.print( cMax2 );

  lcd.setCursor(0,3);
  lcd.print( cMin3 );
  lcd.print( ' ' );
  lcd.print( cMax3 );

  delay(1000);
}


