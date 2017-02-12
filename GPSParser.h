////////////////////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
//
// GPSParser - efficient NMEA string parser
////////////////////////////////////////////////////////////
#ifndef GPSPARSER_H
#define GPSPARSER_H

//#define CGPSParser_DEBUG

#include "GPSDefs.h"
#include "GPSParserData.h"
#include "GPSParserSentences.h"
#include "GPSPos.h"

////////////////////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
//
// Efficient portable parser for GPS strings - uses double
// Handles GPGGA, GPGSA, GPGSV, GPRMC, GPVTG.
// Inspired by:
// TinyGPS (Mikal Hart) https://github.com/mikalhart/TinyGPS
// and
// NMEA library
// NMEA library URL: http://nmea.sourceforge.net)
///////////////////////////////////////////////////////////

// State machine states
typedef enum GPSParser_State_E
{
	GPSParser_S_WaitingForStart = 0,		// Waiting for $
	GPSParser_S_ParsingTerms,			// Processing comma-separated terms
	GPSParser_S_ProcessingChecksum		// Checking the checksum term
} GPSParser_State_T;


// Largest distance between commas and such
#define CGPSParser_TERMSIZE	(16)

// The actual string parser. Data is decoded by
// the individual sentence objects
class CGPSParser
{
protected:

	// Current state machine state
	GPSParser_State_T m_state;

	// Location to accumulate data as the parse
	// progresses. Mostly for the stuff between
	// commas
	char m_term[CGPSParser_TERMSIZE];
	int m_termOffset;
	int m_termNumber;


	// Running checksum
	unsigned char m_checksum;

	// Term processing. This mediates between the
	// simple parser and the more complicated
	// GPS sentence objects
	bool parseChar(unsigned char _c);
	void AddTermChar(unsigned char _c);
	void processTerm();		// Returns true when valid and recognized sentence is complete
	unsigned char from_hex(unsigned char _a);

	// Sentence currently being parsed
	GPSSentenceData_T m_gpsSentenceData;
	GPSSentence_T m_currentSentenceType;
	GPSSentence_T SelectSentence();

	// Accumulated parser data
	CGPSParserData m_GPSData;


#ifdef CGPSParser_DEBUG
	int m_GPSParser_LineNumber;	// We start on line one
	int m_GPSParser_ValidLines;
#endif

public:
	CGPSParser();
	virtual ~CGPSParser();

	void reset();

	// Returns true if any sentences were processed (data may have changed)
	bool parse(const unsigned char _buf[], unsigned int _bufLen);
	CGPSParserData &getGPSData() { return m_GPSData; }

#ifdef CGPSParser_DEBUG
	int getLineNumber() { return m_GPSParser_LineNumber; }
	int getValidLines() { return m_GPSParser_ValidLines; }
#endif
};

#endif
