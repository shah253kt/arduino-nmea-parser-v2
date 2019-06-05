#include "NmeaParserV2.h"
#include "Arduino.h"

NmeaParserV2::NmeaParserV2(Stream &stream)
{
	_stream = &stream;
}

NmeaParserV2::~NmeaParserV2()
{
	delete[] rawStatement;
	delete[] field;
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
		isValid = false;
		currentFieldCount = 0;
		currentRawIndex = 0;
		calculatedChecksum = 0;
		checksumIndex = 0;
		rawStatement[0] = '\0';
		nextField();
	}
	else if (c == ',')
	{
		calculatedChecksum ^= c;
		nextField();
	}
	else if (c == '\r' || c == '\n')
	{
		isValid = strtol(&rawStatement[checksumIndex], NULL, 16) == calculatedChecksum && !isValid;

		if(isValid) {
			nextField();
		}
	}
	else if (c == '*')
	{
		nextField();
		checksumIndex = currentRawIndex + 1;
	}
	else if (checksumIndex == 0)
	{
		calculatedChecksum ^= c;
	}

	// Copy into raw statement char array
	if (currentRawIndex < NmeaParserV2_MAX_RAW_STATEMENT_LENGTH && c != '\r' && c != '\n')
	{
		rawStatement[currentRawIndex] = c;
		rawStatement[++currentRawIndex] = '\0';
	}

	return isValid;
}

void NmeaParserV2::nextField()
{
	fieldIndexes[currentFieldCount++] = currentRawIndex + 1;
}

byte NmeaParserV2::getFieldCount()
{
	return currentFieldCount - 1;
}

char *NmeaParserV2::getField(byte index)
{
	if (index < getFieldCount())
	{
		bytesCount = fieldIndexes[index + 1] - fieldIndexes[index] - 1;
		strncpy(field, &rawStatement[fieldIndexes[index]], bytesCount);
		field[bytesCount] = '\0';

		return field;
	}

	return "";
}

char *NmeaParserV2::getRawStatement()
{
	return rawStatement;
}
