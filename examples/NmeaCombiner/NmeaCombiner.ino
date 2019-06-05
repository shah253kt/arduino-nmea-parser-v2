/**
   Please note that both hardware and software serial suffers from inadequate buffer size
   when dealing with fast data transmission or large amount of data to be processed.
   If you are not getting valid NMEA statement, try to increase the serial buffer size.
   Refer this page for more information: (https://internetofhomethings.com/homethings/?p=927)

   For SoftwareSerial, modify the following file:
   <Arduino installation folder>\hardware\arduino\avr\libraries\SoftwareSerial\SoftwareSerial.h
   and modify the following line:
   #define _SS_MAX_RX_BUFF 64 // RX buffer size

   For HardwareSerial, modify the following file:
   <Arduino installation folder>\hardware\arduino\avr\cores\arduino\HardwareSerial.h
   and modify the following line:
   #define SERIAL_TX_BUFFER_SIZE 64

   However, please note that by increasing those values, you are also increasing the SRAM used
   by the Arduino, limiting the available memory for your sketch.
*/

#include <SoftwareSerial.h>
#include <NmeaParserV2.h>

SoftwareSerial ss(2, 3);
NmeaParserV2 parser1(Serial1);
NmeaParserV2 parser2(Serial2);
NmeaParserV2 parser3(Serial3);
NmeaParserV2 parser4(ss);

void setup()
{
  Serial.begin(9600);
  Serial1.begin(4800);
  Serial2.begin(4800);
  Serial3.begin(4800);
  ss.begin(9600);
}

void loop()
{
	if (Serial1.valid())
	{
		Serial.println(Serial1.getRawStatement());
	}
	
	if (Serial2.valid())
	{
		Serial.println(Serial2.getRawStatement());
	}
	
	if (Serial3.valid())
	{
		Serial.println(Serial3.getRawStatement());
	}
	
	if (ss.valid())
	{
		Serial.println(ss.getRawStatement());
	}
}
