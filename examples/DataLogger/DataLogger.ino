// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"
#include <SD.h>

#define readButton 9
#define CS 10

RTC_DS1307 RTC;
char buffer[50];

File myFile;

void setup () {
    Serial.begin(57600);
    Serial.print("Initializing SD card...");
    pinMode(CS, OUTPUT);
    pinMode(readButton, INPUT);
    
    if (!SD.begin(CS)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");
    
    Wire.begin();
    RTC.begin();
    RTC.sqw(1);		//0 Led off - 1 Freq 1Hz - 2 Freq 4096kHz - 3 Freq 8192kHz - 4 Freq 32768kHz
    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
}

void loop () {
    DateTime now = RTC.now();
    
    sprintf(buffer,  "%02d/%02d/%d %02d:%02d:%02d %d %d %d %d", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second(), analogRead( A0 ), analogRead( A1 ), analogRead( A2 ), analogRead( A3 ) );
    Serial.println( buffer );
    
    myFile = SD.open("dati1.log", FILE_WRITE);
    if (myFile) {
      myFile.println(buffer);
      myFile.close();
    } else {
      Serial.println("error opening data.log");
    }
    
    if ( digitalRead( readButton ) == HIGH ) {
        Serial.println(" ");
        Serial.println("Lettura Log Salvato ----------- ");
        Serial.println(" ");
        myFile = SD.open("dati1.log");
        if (myFile) {
          while (myFile.available()) {
    	    Serial.write(myFile.read());
          }
          myFile.close();
        }
        delay( 5000 );
    }
    
    delay(1000);
}
