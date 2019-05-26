#ifndef NmeaParserV2_H_INCLUDED
#define NmeaParserV2_H_INCLUDED

/**
 * This value determines maximum number of fields in an NMEA statement.
 * Most standard NMEA statement has less than 16 fields, so this default value is more than enough.
 * If you really need to reduce memory usage or has NMEA statements that has more than 16 fields,
 * you may overwrite this value.
 * Valid values: 1 to 255 (inclusive)
 * Default value: 16
 */
#ifndef NmeaParserV2_MAX_FIELD_COUNT
#define NmeaParserV2_MAX_FIELD_COUNT 16
#endif

/**
 * This value determines maximum character length in each field.
 * Most standard NMEA statement has less than 15 characters in a field, so this default value is enough.
 * If you really need to reduce memory usage or has fields that has more than 15 characters,
 * you may overwrite this value.
 * Valid values: 1 to 255 (inclusive)
 * Default value: 15
 */
#ifndef NmeaParserV2_MAX_FIELD_LENGTH
#define NmeaParserV2_MAX_FIELD_LENGTH 15
#endif

#ifndef NmeaParserV2_MAX_RAW_MESSAGE_LENGTH
#define NmeaParserV2_MAX_RAW_MESSAGE_LENGTH ((NmeaParserV2_MAX_FIELD_COUNT * NmeaParserV2_MAX_FIELD_LENGTH) + NmeaParserV2_MAX_FIELD_COUNT + 6)
#endif

#ifndef NmeaParserV2_MAX_SEND_MESSAGE_LENGTH
#define NmeaParserV2_MAX_SEND_MESSAGE_LENGTH 20
#endif

#ifndef stricmp
#define stricmp(a, b) strcasecmp(a, b)
#endif

#include "Arduino.h"

class NmeaParserV2
{
private:
	Stream *_stream;
	char c;
	char fields[NmeaParserV2_MAX_FIELD_COUNT][NmeaParserV2_MAX_FIELD_LENGTH];
	char rawMessage[NmeaParserV2_MAX_RAW_MESSAGE_LENGTH + 1];
	char hexString[3], buffer[3];
	byte currentFieldCountIndex, currentFieldIndex, currentRawIndex, checksum;
	bool isChecksumField, isValid;

	bool encode(char c);
	void appendField(char c);
	void nextField();
	char *toHexString(byte value);

public:
	NmeaParserV2(Stream &stream);
	~NmeaParserV2();

	/**
	 * Reads all available serial bytes until a packet is parsed, an error occurs, or the buffer is empty.
	 * Returns true if packet is valid.
	 */
	bool valid();

	byte getFieldCount();

	char *getField(byte index);

	char *getRawMessage();

	// template <typename T>
	// bool append(T item)
	// {
	// 	return false;
	// }

	// void send();
};

#endif
