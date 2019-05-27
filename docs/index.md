# Arduino NMEA Parser

*Arduino NMEA Parser is a library created to simplify the process of verifying and extracting all fields in an NMEA 0183 statement being transmitted through a serial stream.*

## Usage

To use the library:

1. Download and install the library in Arduino default library folder:

    [Refer to this tutorial](https://www.arduino.cc/en/Guide/Libraries)

2. Include the library in your sketch:

    ```cpp
    #include <NmeaParserV2.h>
    ```

3. Initialize the NmeaParserV2 object:

    ```cpp
    // If you are using hardware serial:
    NmeaParserV2 parser(Serial1);

    // If you are using SoftwareSerial:
    SoftwareSerial ss(2, 3);
    NmeaParserV2 parser(ss);
    ```

4. Check if any valid NMEA statement is available to be processed:
    ```cpp
    void loop() {
        if(parser.valid()) {
            // Do something here...
        }
    }
    ```

## API

* `bool valid()`
    Reads all available serial bytes until a statement is parsed or the buffer is empty. Returns true if statement is valid.

* `byte getFieldCount()`
    Returns number of fields parsed.

* `char *getField(byte index)`
    Returns a field.

* `char *getRawStatement()`
    Returns a raw NMEA statement.

## Constants

*You may modify the following constants if you need to, but remember that this will also increase memory usage quite significantly.*

`NmeaParserV2_MAX_FIELD_COUNT` (default: 16)
This value determines maximum number of fields in an NMEA statement.

`NmeaParserV2_MAX_FIELD_LENGTH` (default: 15)
This value determines maximum character length in each field.

## Others

Please note that both hardware and software serial suffers from inadequate buffer size when dealing with fast data transmission or large amount of data to be processed. If you are not getting valid NMEA statement, try to increase the serial buffer size. [Refer this page](https://internetofhomethings.com/homethings/?p=927) for more information.

Basically, you need to increase the buffer size of the streams.

For SoftwareSerial, modify the following file:
    *<Arduino installation folder>\hardware\arduino\avr\libraries\SoftwareSerial\SoftwareSerial.h* and modify the following line:
    ```cpp
    #define _SS_MAX_RX_BUFF 64 // From this
    #define _SS_MAX_RX_BUFF 256 // To this
    ```

For HardwareSerial, modify the following file:
    *<Arduino installation folder>\hardware\arduino\avr\cores\arduino\HardwareSerial.h* and modify the following line:
    ```cpp
    #define SERIAL_TX_BUFFER_SIZE 64 // From this
    #define SERIAL_TX_BUFFER_SIZE 256 // To this
    ```

However, please note that by increasing those values, you are also increasing the SRAM used by the Arduino, limiting the available memory for your sketch.
