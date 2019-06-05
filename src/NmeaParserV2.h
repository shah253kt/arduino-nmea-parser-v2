#ifndef NmeaParserV2_H_INCLUDED
#define NmeaParserV2_H_INCLUDED

/**
 * This value determines maximum number of fields in an NMEA statement.
 * Most standard NMEA statement has less than 16 fields, so this default value is more than enough.
 * If you really need to reduce memory usage or has NMEA statements that has more than 20 fields,
 * you may overwrite this value.
 * Valid values: 1 to 255 (inclusive)
 * Default value: 20
 */
#ifndef NmeaParserV2_MAX_FIELD_COUNT
#define NmeaParserV2_MAX_FIELD_COUNT 20
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

#ifndef NmeaParserV2_MAX_RAW_STATEMENT_LENGTH
#define NmeaParserV2_MAX_RAW_STATEMENT_LENGTH 270
#endif

#ifndef NmeaParserV2_MAX_SEND_STATEMENT_LENGTH
#define NmeaParserV2_MAX_SEND_STATEMENT_LENGTH 270
#endif

#ifndef stricmp
#define stricmp(a, b) strcasecmp(a, b)
#endif

#include "Arduino.h"

class NmeaParserV2
{
private:
	Stream *_stream;

	// For sentence decoding
	byte fieldIndexes[NmeaParserV2_MAX_FIELD_COUNT];
	char rawStatement[NmeaParserV2_MAX_RAW_STATEMENT_LENGTH];
	char field[NmeaParserV2_MAX_FIELD_LENGTH];
	byte currentFieldCount, currentRawIndex, calculatedChecksum, checksumIndex;
	bool isValid;
	size_t bytesCount;

	bool encode(char c);
	void appendField(char c);
	void nextField();

	// For sentence encoding
	char statementToSend[NmeaParserV2_MAX_SEND_STATEMENT_LENGTH];
	byte sendStatementChecksum, currentSendStatementIndex;
	char hexString[3], buffer[NmeaParserV2_MAX_FIELD_LENGTH];
	
	char *toHexString(byte value);
	void insertComma();
	void scanBuffer();

public:
	NmeaParserV2(Stream &stream);
	~NmeaParserV2();

	/**
	 * Reads all available serial bytes until a statement is parsed or the buffer is empty.
	 * Returns true if statement is valid.
	 */
	bool valid();

	/**
	 * Returns number of fields parsed.
	 */
	byte getFieldCount();

	/**
	 * Returns a field.
	 */
	char *getField(byte index);

	/**
	 * Returns a raw NMEA statement.
	 */
	char *getRawStatement();

	void prepareStatement();

	void append(char *cArray, byte length);
	void append(int value);
	void append(long value);
	void append(double value, byte totalLength, byte mantissaLength);

	void send();
};

#endif
