int MAPWM = 5;
int MA1 = 6;
int MA2 = 7;

int MBPWM = 9;
int MB1 = 10;
int MB2 = 11;


void setup() {
  pinMode( 13, OUTPUT );

  pinMode( MAPWM, OUTPUT );
  pinMode( MA1, OUTPUT );
  pinMode( MA2, OUTPUT );

  pinMode( MBPWM, OUTPUT );
  pinMode( MB1, OUTPUT );
  pinMode( MB2, OUTPUT );

}

void loop() {
  
/*  
  analogWrite( MAPWM, 127 );
  digitalWrite( MA1, HIGH );
  digitalWrite( MA2, LOW );
  analogWrite( MBPWM, 127 );
  digitalWrite( MB1, HIGH );
  digitalWrite( MB2, LOW );

  digitalWrite( 13, HIGH );
  delay( 50 );
  digitalWrite( 13, LOW );
  delay( 3000 );
*/

  analogWrite( MAPWM, 255 );
  digitalWrite( MA1, LOW );
  digitalWrite( MA2, HIGH );
  analogWrite( MBPWM, 255 );
  digitalWrite( MB1, LOW );
  digitalWrite( MB2, HIGH );

  digitalWrite( 13, HIGH );

  delay( 3000 );

  digitalWrite( MA1, HIGH );
  digitalWrite( MA2, LOW );
  digitalWrite( MB1, HIGH );
  digitalWrite( MB2, LOW );

//  analogWrite( MAPWM, 0 );
//  analogWrite( MBPWM, 0 );

  digitalWrite( 13, LOW );
  delay( 3000 );
}



