#include "NmeaParserV2.h"
#include "Arduino.h"

NmeaParserV2::NmeaParserV2(Stream &stream)
{
	_stream = &stream;
	prepareStatement();
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

		if (isValid)
		{
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

// For sentence encoding

void NmeaParserV2::prepareStatement()
{
	statementToSend[0] = '$';
	statementToSend[1] = '\0';
	sendStatementChecksum = 0;
	currentSendStatementIndex = 1;
}

void NmeaParserV2::append(char *cArray, byte length)
{
	insertComma();

	for (byte i = 0; i < length; i++)
	{
		sendStatementChecksum ^= cArray[i];
		statementToSend[currentSendStatementIndex++] = cArray[i];
	}
}

void NmeaParserV2::append(int value)
{
	append((long) value);
}

void NmeaParserV2::append(long value)
{
	insertComma();
	itoa(value, buffer, 10);
	scanBuffer();
}

void NmeaParserV2::append(double value, byte totalLength, byte mantissaLength)
{
	insertComma();
	dtostrf(value, totalLength, mantissaLength, buffer);
	scanBuffer();
}

void NmeaParserV2::send()
{
	toHexString(sendStatementChecksum);
	statementToSend[currentSendStatementIndex++] = '*';
	statementToSend[currentSendStatementIndex++] = hexString[0];
	statementToSend[currentSendStatementIndex++] = hexString[1];
	statementToSend[currentSendStatementIndex++] = '\0';
	_stream->println(statementToSend);
	prepareStatement();
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

	for (byte i = 0; i < strlen(hexString); i++)
	{
		hexString[i] = toupper(hexString[i]);
	}

	return hexString;
}

void NmeaParserV2::insertComma()
{
	if (currentSendStatementIndex > 1)
	{
		sendStatementChecksum ^= ',';
		statementToSend[currentSendStatementIndex++] = ',';
	}
}

void NmeaParserV2::scanBuffer()
{
	for (byte i = 0; i < strlen(buffer); i++)
	{
		sendStatementChecksum ^= buffer[i];
		statementToSend[currentSendStatementIndex++] = buffer[i];
	}
}
