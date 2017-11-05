// AW: odczyt bieżący i średnie z ostatnich kolejnych minut


#include <OneWire.h>
#include <LiquidCrystal_I2C.h>

#define SERIAL_READ_TEMP

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
#endif
  
    if ( OneWire::crc8( addr, 7) != addr[7]) {
        Serial.print("CRC is not valid!\n");
        continue;
    }
  
    if ( addr[0] == 0x10) {
#ifdef SERIAL_READ_TEMP
        Serial.print("Device is a DS18S20 family device.  ");
#endif
    }
    else if ( addr[0] == 0x28) {
#ifdef SERIAL_READ_TEMP
        Serial.print("Device is a DS18B20 family device.  ");
#endif
    }
    else {
        Serial.print("Device family is not recognized: 0x");
        Serial.println(addr[0],HEX);
        continue;
    }
  
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

#define AVG_CNT 19
#define AVG_PERIOD 60

int avgTemp[AVG_CNT];
int avgPtr = 0;

int prdSum = 0;
int prdCnt = 0;

void setup()
{      
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  readAllThermometers();
  for( int i=0; i<AVG_CNT; i++ )
    avgTemp[ i ] = 0;
}

void readTemp() {
  lcd.setCursor( 0, 0 );
  lcd.print( "*" );
  readAllThermometers();
  lcd.setCursor( 0, 0 );
  lcd.print( " " );
}

void loop() {
  readTemp();
  int temp = (minTemp + maxTemp) / 2 / 10;
  lcd.setCursor( 1, 0 );
  lcd.print( temp );
  lcd.print( " " ); 
//  Serial.println( temp );

  Serial.print( "lcd: " );
  for( int i=0; i<AVG_CNT; i++ ) {
    int col = ((i+1)%5)*4+1;
    int row = (i+1)/5;
    lcd.setCursor( col, row );
    lcd.print( avgTemp[(avgPtr-1-i+AVG_CNT)%AVG_CNT]);
    lcd.print( " " );
    Serial.print( avgTemp[(avgPtr-1-i+AVG_CNT)%AVG_CNT]);
    Serial.print( " " );
  }
  Serial.println();
  
  prdSum += temp;
//  Serial.println( prdSum );
  prdCnt++;
  if( prdCnt >= AVG_PERIOD ) {
    int avg = prdSum / prdCnt;
    prdSum = 0;
    prdCnt = 0;
//    Serial.print( "/// " );
//    Serial.println( avg );
    prdCnt = 0;

    avgTemp[avgPtr] = avg;
    avgPtr++;
    if( avgPtr >= AVG_CNT )
      avgPtr = 0;

    for( int i=0; i<AVG_CNT; i++ ) {
      Serial.print( avgTemp[ i ]);
      Serial.print( " " );
    }
    Serial.println();
  
  }
  
  delay(1000);
}


