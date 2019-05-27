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
