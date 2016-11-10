////////////////////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
//
// Efficient portable parser for GPS strings - uses double
// Handles GPGGA, GPGSA, GPGSV, GPRMC, GPVTG.
// Inspired by:
// TinyGPS (Mikal Hart) https://github.com/mikalhart/TinyGPS
// and
// NMEA parser
// NMEA library URL: http://nmea.sourceforge.net)
///////////////////////////////////////////////////////////
#include <string.h>
#include <stdlib.h>

#include "GPSParser.h"

////////////////////////////////////////////////////////////
// Efficient portable parser for GPS strings - uses double
// Handles GPGGA, GPGSA, GPGSV, GPRMC, GPVTG
////////////////////////////////////////////////////////////
CGPSParser::CGPSParser()
{
	reset();
}

CGPSParser::~CGPSParser()
{

}

// Returns true if any sentences were processed (data may have changed)
bool CGPSParser::parse(const unsigned char _buf[], unsigned int _bufLen)
{
	bool processedSentence = false;
	unsigned int index;
	const unsigned char *cPtr = _buf;

	// Sanity
	if((_buf == 0) || (_bufLen == 0))
		return 0;

	// parse it one character at a time.
	for(index = 0; index < _bufLen; ++index)
		processedSentence |= parseChar(*cPtr++);

	// Return the number of strings successfully processed
	return processedSentence;
}

// This method was inspired by Mikal Hart's TinyGPS
bool CGPSParser::parseChar(unsigned char _c)
{
	// Did we get something?
	bool validSentence = false;

	// Screen out the newline character because we don't use it
	if(_c == '\n')
		return false;

	// Extremely special case for very (I mean VERY) noisy data streams)
	if((m_state != GPSParser_S_WaitingForStart) &&
		(_c == '$'))
	{
		// We have received a '$' when we were not expecting one, so the data must be a mess.
		// We'll just reset the parser and continue as though this '$' is the start of the
		// sentence.
		m_state = GPSParser_S_WaitingForStart;
	}

	// How the characters are handled depends on which
	// state we are in.
	switch(m_state)
	{
		// Waiting for $
		case GPSParser_S_WaitingForStart:
			if(_c == '$')
			{
				// Prep for parsing terms
				reset();

				// Change state to term processing
				m_state = GPSParser_S_ParsingTerms;
			}
			break;

		// Processing comma-separated terms
		case GPSParser_S_ParsingTerms:
			switch(_c)
			{
				// Comma and asterisk both end the current term, so we do some
				// common processing to reduce code size. However, asterisk causes
				// a state change
				case ',':
					// Process this term
					m_checksum ^= _c;
					// Fall through

				case '*':
					processTerm();

					// And start the next
					m_termOffset = 0;
					m_term[m_termOffset] = '\0';
					m_termNumber++;

					// Star also ends the current term, and it changes state to checksum processing
					if(_c == '*')
						m_state = GPSParser_S_ProcessingChecksum;
					break;

				default:
					m_checksum ^= _c;
					AddTermChar(_c);
					break;
			}
			break;

		// Checking the checksum term
		case GPSParser_S_ProcessingChecksum:
			if(_c == '\r')	// This ends the current GPS string so do EOL processing
			{
				// First, verify the checksum
				if(m_termOffset == 2)
				{
					// Check the sentence checksum against ours
					unsigned char checksum = 16 * from_hex(m_term[0]) + from_hex(m_term[1]);
					if(checksum == m_checksum)
					{
						validSentence = true;

						// This sentence is complete, so copy the data
						switch(m_currentSentenceType)
						{
							default:
							case GPSSentence_T_Unknown:
								validSentence = false;
								break;

#if (GPSParser_INCLUDE_GPGGA)
							case GPSSentence_T_GPGGA:
								CGPSSentence_GPGGA::transferData(m_GPSData, m_gpsSentenceData);
								break;
#endif
#if (GPSParser_INCLUDE_GPRMC)
							case GPSSentence_T_GPRMC:
								CGPSSentence_GPRMC::transferData(m_GPSData, m_gpsSentenceData);
								break;
#endif
#if (GPSParser_INCLUDE_GPGSA)
							case GPSSentence_T_GPGSA:
								CGPSSentence_GPGSA::transferData(m_GPSData, m_gpsSentenceData);
								break;
#endif
#if (GPSParser_INCLUDE_GPGSV)
							case GPSSentence_T_GPGSV:
								CGPSSentence_GPGSV::transferData(m_GPSData, m_gpsSentenceData);
								break;
#endif
#if (GPSParser_INCLUDE_GPVTG)
							case GPSSentence_T_GPVTG:
								CGPSSentence_GPVTG::transferData(m_GPSData, m_gpsSentenceData);
								break;
#endif
						}
					}
				}

				// reset the parser
				reset();

				// And start looking for the next one
				m_state = GPSParser_S_WaitingForStart;
			}
			else
			{
				// Not the end of the string, so keep processing characters
				AddTermChar(_c);
			}
			break;

		// Invalid state... we should never get here
		default:
			break;
	}

#ifdef CGPSParser_DEBUG
	// Line number tracking for diagnostics
	if(_c == '\r')
	{
		// Bump the line number
		m_GPSParser_LineNumber++;
	}
#endif

	return validSentence;
}

// Add character to accumulating term, but do not
// overrun the buffer
void CGPSParser::AddTermChar(unsigned char _c)
{
	if(m_termOffset < (CGPSParser_TERMSIZE - 1))
	{
		m_term[m_termOffset++] = _c;
		m_term[m_termOffset] = '\0';
	}
}

void CGPSParser::processTerm()
{
	bool termProcessedOk = false;

	// Screen to reduce processing
	if((m_termNumber != 0) && (m_currentSentenceType == GPSSentence_T_Unknown))
		return;

	// If this is the first term then it is the
	// sentence type, so create the sentence
	if(m_termNumber == 0)
	{
		m_currentSentenceType = SelectSentence();
		return;
	}

	// Screen for unknown sentences
	if(m_currentSentenceType == GPSSentence_T_Unknown)
		return;

	switch(m_currentSentenceType)
	{
		default:
		case GPSSentence_T_Unknown:
			return;

#if (GPSParser_INCLUDE_GPGGA)
		case GPSSentence_T_GPGGA:
			termProcessedOk = CGPSSentence_GPGGA::processTerm(m_term, m_termNumber, m_gpsSentenceData);
			break;
#endif
#if (GPSParser_INCLUDE_GPRMC)
		case GPSSentence_T_GPRMC:
			termProcessedOk = CGPSSentence_GPRMC::processTerm(m_term, m_termNumber, m_gpsSentenceData);
			break;
#endif
#if (GPSParser_INCLUDE_GPGSA)
		case GPSSentence_T_GPGSA:
			termProcessedOk = CGPSSentence_GPGSA::processTerm(m_term, m_termNumber, m_gpsSentenceData);
			break;
#endif
#if (PSParser_INCLUDE_GPGSV)
		case GPSSentence_T_GPGSV:
			termProcessedOk = CGPSSentence_GPGSV::processTerm(m_term, m_termNumber, m_gpsSentenceData);
			break;
#endif
#if (GPSParser_INCLUDE_GPVTG)
		case GPSSentence_T_GPVTG:
			termProcessedOk = CGPSSentence_GPVTG::processTerm(m_term, m_termNumber, m_gpsSentenceData);
			break;
#endif
	}

	if(termProcessedOk == false)
	{
		// The sentence rejected the term, so this is not
		// going to work.
		m_currentSentenceType = GPSSentence_T_Unknown;
	}
}

// Check the term and create a sentence object from it
GPSSentence_T CGPSParser::SelectSentence()
{
#if (GPSParser_INCLUDE_GPGGA)

	if(strcmp((const char *)m_term, "GPGGA") == 0)
	{
		CGPSSentence_GPGGA::init(m_gpsSentenceData);
		return GPSSentence_T_GPGGA;
	}
#endif
#if (GPSParser_INCLUDE_GPRMC)
	if(strcmp((const char *)m_term, "GPRMC") == 0)
	{
		CGPSSentence_GPRMC::init(m_gpsSentenceData);
		return GPSSentence_T_GPRMC;
	}
#endif
#if (GPSParser_INCLUDE_GPGSA)
	if(strcmp((const char *)m_term, "GPGSA") == 0)
	{
		CGPSSentence_GPGSA::init(m_gpsSentenceData);
		return GPSSentence_T_GPGSA;
	}
#endif
#if (GPSParser_INCLUDE_GPGSV)
	if(strcmp((const char *)m_term, "GPGSV") == 0)
	{
		CGPSSentence_GPGSV::init(m_gpsSentenceData);
		return GPSSentence_T_GPGSV;
	}
#endif
#if (GPSParser_INCLUDE_GPVTG)
	if(strcmp((const char *)m_term, "GPVTG") == 0)
	{
		CGPSSentence_GPVTG::init(m_gpsSentenceData);
		return GPSSentence_T_GPVTG;
	}
#endif
	// Unknown sentence
	return GPSSentence_T_Unknown;
}

void CGPSParser::reset()
{
	m_state = GPSParser_S_WaitingForStart;

	m_termOffset = 0;
	m_term[m_termOffset] = '\0';
	m_termNumber = 0;

	m_checksum = 0;

	m_currentSentenceType = GPSSentence_T_Unknown;

#ifdef CGPSParser_DEBUG
	m_GPSParser_LineNumber = 1;
#endif
}

unsigned char CGPSParser::from_hex(unsigned char _a)
{
  if (_a >= 'A' && _a <= 'F')
    return _a - 'A' + 10;
  else if (_a >= 'a' && _a <= 'f')
    return _a - 'a' + 10;
  else
    return _a - '0';
}
