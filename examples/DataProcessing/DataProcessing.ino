/**
 * Please note that both hardware and software serial suffers from inadequate buffer size
 * when dealing with fast data transmission or large amount of data to be processed.
 * If you are not getting valid NMEA statement, try to increase the serial buffer size.
 * Refer this page for more information: (https://internetofhomethings.com/homethings/?p=927)
 * 
 * For SoftwareSerial, modify the following file:
 * <Arduino installation folder>\hardware\arduino\avr\libraries\SoftwareSerial\SoftwareSerial.h
 * and modify the following line:
 * #define _SS_MAX_RX_BUFF 64 // RX buffer size
 * 
 * For HardwareSerial, modify the following file:
 * <Arduino installation folder>\hardware\arduino\avr\cores\arduino\HardwareSerial.h
 * and modify the following line:
 * #define SERIAL_TX_BUFFER_SIZE 64
 * 
 * However, please note that by increasing those values, you are also increasing the SRAM used
 * by the Arduino, limiting the available memory for your sketch.
 */

#include <SoftwareSerial.h>
#include <NmeaParserV2.h>

SoftwareSerial ss(2, 3);
NmeaParserV2 parser(ss);

void setup()
{
	Serial.begin(9600);
	ss.begin(4800);
}

void loop()
{
	if (parser.valid())
	{
		if(strcmp(parser.getField(0), "GPGGA") == 0) {
			Serial.println(parser.getRawStatement());

			// You may use char array functions like strcmp, strcpy, atoi, atof, etc. to process the value of each field.
		}
	}
}
