#include "NmeaParserV2.h"
#include "Arduino.h"

NmeaParserV2::NmeaParserV2(Stream &stream)
{
	_stream = &stream;
}

NmeaParserV2::~NmeaParserV2()
{
	delete[] fields;
	delete[] rawStatement;
	delete[] hexString;
}

bool NmeaParserV2::valid()
{
	while (_stream->available())
	{
		if (encode(_stream->read()))
		{
			break;
		}
	}

	return isValid;
}

bool NmeaParserV2::encode(char c)
{
	if (c == '$')
	{
		isChecksumField = false;
		isValid = false;
		currentFieldCountIndex = 0;
		currentFieldIndex = 0;
		currentRawIndex = 0;
		checksum = 0;
		rawStatement[0] = '\0';
	}
	else if (c == ',')
	{
		checksum ^= c;
		nextField();
	}
	else if (c == '\r' || c == '\n')
	{
		isValid = stricmp(toHexString(checksum), fields[currentFieldCountIndex]) == 0 && !isValid;
		isChecksumField = false;
	}
	else if (isChecksumField)
	{
		appendField(c);
	}
	else if (c == '*')
	{
		isChecksumField = true;
		nextField();
	}
	else
	{
		checksum ^= c;
		appendField(c);
	}

	if (currentRawIndex < NmeaParserV2_MAX_RAW_STATEMENT_LENGTH)
	{
		rawStatement[currentRawIndex] = c;
		rawStatement[++currentRawIndex] = '\0';
	}

	return isValid;
}

void NmeaParserV2::appendField(char c)
{
	fields[currentFieldCountIndex][currentFieldIndex] = c;
	fields[currentFieldCountIndex][++currentFieldIndex] = '\0';
}

void NmeaParserV2::nextField()
{
	fields[currentFieldCountIndex++][currentFieldIndex] = '\0'; // Terminate current field
	currentFieldIndex = 0;
}

char *NmeaParserV2::toHexString(byte value)
{
	itoa(value, buffer, 16);

	if (strlen(buffer) < 2)
	{
		strcpy(hexString, "0\0");
	}
	else
	{
		hexString[0] = '\0';
	}

	strncat(hexString, buffer, strlen(buffer));

	return hexString;
}

byte NmeaParserV2::getFieldCount()
{
	return currentFieldCountIndex + 1;
}

char *NmeaParserV2::getField(byte index)
{
	if (index < NmeaParserV2_MAX_FIELD_COUNT)
	{
		return fields[index];
	}

	return "";
}

char *NmeaParserV2::getRawStatement()
{
	return rawStatement;
}
