////////////////////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
//
// GPS Parser Sentences
////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <string.h>

#include "GPSParser.h"

/*
static int GPSSentence_Convert_Int_Data(const char _term[])
{
	// If the term is empty then return GPS_INVALID_DATA
	if(_term[0] == 0)
		return GPS_INVALID_DATA;

	return atoi(_term);
}
*/

static long GPSSentence_Convert_Long_Data(const char _term[])
{
	// If the term is empty then return GPS_INVALID_DATA
	if(_term[0] == 0)
		return GPS_INVALID_DATA;

	return atol(_term);
}

static double GPSSentence_Convert_Double_Data(const char _term[])
{
	// If the term is empty then return GPS_INVALID_DATA
	if(_term[0] == 0)
		return GPS_INVALID_DATA;

	return atof(_term);
}

// Taken directly from NMEA library nmea_ndeg2degree(gmath.c)
static double GPSSentence_Convert_LL_to_DD(const char _term[])
{
	// If the term is empty then return GPS_INVALID_DATA
	if(_term[0] == 0)
		return GPS_INVALID_DATA;

	double ll = atof(_term);

	double deg = ((int)(ll / 100));
	ll = deg + (ll - deg * 100.) / 60.;

	return ll;
}

static void GPSSentence_Transfer_Int_Data(int &_from, int &_to)
{
	if(GPS_IS_VALID_DATA(_from))
		_to = _from;
	else
		_to = GPS_INVALID_DATA;
}

static void GPSSentence_Transfer_Double_Data(double &_from, double &_to)
{
	if(GPS_IS_VALID_DATA(_from))
		_to = _from;
	else
		_to = GPS_INVALID_DATA;
}
////////////////////////////////////////////////////////////
// The actual GPS sentence classes
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// We create objects to handle each sentence type and
// they have to be defined here because we use them
// later and need to know what they are
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//	Global Positioning System Fix Data. Time, position and fix related data for a GPS receiver.
//
//	eg2. $--GGA,hhmmss.ss,ddmm.mmmm,a,dddmm.mmmm,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx
//
//	hhmmss.ss = UTC of position
//	ddmm.mmmm = latitude of position
//	a = N or S
//	dddmm.mmmm = Longitude of position
//	a = E or W
//	x = GPS Quality indicator (0=no fix, 1=GPS fix, 2=Dif. GPS fix)
//	xx = number of satellites in use
//	x.x = horizontal dilution of precision
//	x.x = Antenna altitude above mean-sea-level
//	M = units of antenna altitude, meters
//	x.x = Geoidal separation
//	M = units of geoidal separation, meters
//	x.x = Age of Differential GPS data (seconds)
//	xxxx = Differential reference station ID
//
//	eg3. $GPGGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh
//	1    = UTC of Position
//	2    = Latitude
//	3    = N or S
//	4    = Longitude
//	5    = E or W
//	6    = GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
//	7    = Number of satellites in use [not those in view]
//	8    = Horizontal dilution of position
//	9    = Antenna altitude above/below mean sea level (geoid)
//	10   = Meters  (Antenna height unit)
//	11   = Geoidal separation (Diff. between WGS-84 earth ellipsoid and
//		   mean sea level.  -=geoid is below WGS-84 ellipsoid)
//	12   = Meters  (Units of geoidal separation)
//	13   = Age in seconds since last update from diff. reference station
//	14   = Diff. reference station ID#
//	15   = Checksum
////////////////////////////////////////////////////////////
#if (GPSParser_INCLUDE_GPGGA)
void CGPSSentence_GPGGA::init(GPSSentenceData_T &_sentenceData)
{
	_sentenceData.GPGGA.m_fixQuality = GPS_INVALID_DATA;
	_sentenceData.GPGGA.m_nSats = GPS_INVALID_DATA;
	_sentenceData.GPGGA.m_dgpsStationID = GPS_INVALID_DATA;

	_sentenceData.GPGGA.m_utc = GPS_INVALID_DATA;
	_sentenceData.GPGGA.m_lat = GPS_INVALID_DATA;
	_sentenceData.GPGGA.m_lon = GPS_INVALID_DATA;
	_sentenceData.GPGGA.m_hDOP = GPS_INVALID_DATA;
	_sentenceData.GPGGA.m_altitude = GPS_INVALID_DATA;
	_sentenceData.GPGGA.m_heightAboveWGS84 = GPS_INVALID_DATA;
	_sentenceData.GPGGA.m_timeSinceLastDGPSUpdate = GPS_INVALID_DATA;

	_sentenceData.GPGGA.m_north = false;
	_sentenceData.GPGGA.m_east = false;
}

bool CGPSSentence_GPGGA::processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData)
{
	switch(_termNumber)
	{
	case 1:		// UTC of Position
		_sentenceData.GPGGA.m_utc = GPSSentence_Convert_Double_Data(_term);
		break;

	case 2:		// Latitude
		_sentenceData.GPGGA.m_lat = GPSSentence_Convert_LL_to_DD(_term);
		break;

	case 3:		// N or S
		if(_term[0] == 'N')
			_sentenceData.GPGGA.m_north = true;
		else
			_sentenceData.GPGGA.m_north = false;
		break;

	case 4:		// Longitude
		_sentenceData.GPGGA.m_lon = GPSSentence_Convert_LL_to_DD(_term);

		break;

	case 5:		// E or W
		if(_term[0] == 'E')
			_sentenceData.GPGGA.m_east = true;
		else
			_sentenceData.GPGGA.m_east = false;
		break;

	case 6:		// GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
		_sentenceData.GPGGA.m_fixQuality = GPSSentence_Convert_Int_Data(_term);
		break;

	case 7:		// Number of satellites in use [not those in view]
		_sentenceData.GPGGA.m_nSats = GPSSentence_Convert_Int_Data(_term);
		break;

	case 8:		// Horizontal dilution of position
		_sentenceData.GPGGA.m_hDOP = GPSSentence_Convert_Double_Data(_term);
		break;

	case 9:		// Antenna altitude above/below mean sea level (geoid)
		_sentenceData.GPGGA.m_altitude = GPSSentence_Convert_Double_Data(_term);
		break;

	case 10:	// Always "M"
		break;

	case 11:	// Geoidal separation (Diff. between WGS-84 earth ellipsoid and mean sea level. -=geoid is below WGS-84 ellipsoid)
		_sentenceData.GPGGA.m_heightAboveWGS84 = GPSSentence_Convert_Double_Data(_term);
		break;

	case 12:	// Always "M"
		break;

	case 13:	// Age in seconds since last update from diff. reference station
		_sentenceData.GPGGA.m_timeSinceLastDGPSUpdate = GPSSentence_Convert_Int_Data(_term);
		break;

	case 14:	// Diff. reference station ID#
		_sentenceData.GPGGA.m_dgpsStationID = GPSSentence_Convert_Int_Data(_term);
		break;

	default:
		return false;
	}
	return true;
}

void CGPSSentence_GPGGA::transferData(CGPSParserData &_gpsParserData, GPSSentenceData_T &_sentenceData)
{
	// Time
	if(GPS_IS_VALID_DATA(_sentenceData.GPGGA.m_utc))
	{
		double time = _sentenceData.GPGGA.m_utc;

		double hour = (int)(time / 10000);
		time -= (hour * 10000);

		double minute = (int)(time / 100);
		time -= (minute * 100);

		double second = (int)time;

		_gpsParserData.m_time.m_hour = (int)hour;
		_gpsParserData.m_time.m_minute = (int)minute;
		_gpsParserData.m_time.m_second = (int)second;
	}
	else
	{
		_gpsParserData.m_time.m_hour = GPS_INVALID_DATA;
		_gpsParserData.m_time.m_minute = GPS_INVALID_DATA;
		_gpsParserData.m_time.m_second = GPS_INVALID_DATA;
	}

	// Lat
	if(GPS_IS_VALID_DATA(_sentenceData.GPGGA.m_lat))
	{
		double lat = _sentenceData.GPGGA.m_lat *
					 (_sentenceData.GPGGA.m_north ? 1 : -1);
		_gpsParserData.m_position.m_lat = lat;
	}
	else
	{
		_gpsParserData.m_position.m_lat = GPS_INVALID_DATA;
	}

	// Lon
	if(GPS_IS_VALID_DATA(_sentenceData.GPGGA.m_lon))
	{
		double lat = _sentenceData.GPGGA.m_lon *
					 (_sentenceData.GPGGA.m_east ? 1 : -1);
		_gpsParserData.m_position.m_lon = lat;
	}
	else
	{
		_gpsParserData.m_position.m_lon = GPS_INVALID_DATA;
	}

	// Fix Quality
	if(GPS_IS_VALID_DATA(_sentenceData.GPGGA.m_fixQuality))
	{
		_gpsParserData.m_fixQuality = _sentenceData.GPGGA.m_fixQuality;
		if(_sentenceData.GPGGA.m_fixQuality > 0)
			_gpsParserData.m_GPSLocked = true;
	}
	else
	{
		_gpsParserData.m_fixQuality = 0;
		_gpsParserData.m_GPSLocked = false;
	}


	// Number of sats
	GPSSentence_Transfer_Int_Data(_sentenceData.GPGGA.m_nSats, _gpsParserData.m_nSatellites);

	// hDOP
	if(GPS_IS_VALID_DATA(_sentenceData.GPGGA.m_hDOP))
	{
		_gpsParserData.m_accuracyHorizontal = _sentenceData.GPGGA.m_hDOP * GPS_NMEA_DOP_FACTOR;

	}
	else
	{
		_gpsParserData.m_accuracyHorizontal = GPS_INVALID_DATA;
	}

	// Altitude
	GPSSentence_Transfer_Double_Data(_sentenceData.GPGGA.m_altitude,
									 _gpsParserData.m_altitude);

	// Time since last differential update
	GPSSentence_Transfer_Int_Data(_sentenceData.GPGGA.m_timeSinceLastDGPSUpdate,
								  _gpsParserData.m_diffFixAge);

	// Differential GPS station ID
	GPSSentence_Transfer_Int_Data(_sentenceData.GPGGA.m_dgpsStationID,
								  _gpsParserData.m_diffStationID);
}
#endif

////////////////////////////////////////////////////////////
//	$GPRMC
//
//	Recommended minimum specific GPS/Transit data
//
//	eg1. $GPRMC,081836,A,3751.65,S,14507.36,E,000.0,360.0,130998,011.3,E*62
//	eg2. $GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68
//
//
//			   225446       Time of fix 22:54:46 UTC
//			   A            Navigation receiver warning A = OK, V = warning
//			   4916.45,N    Latitude 49 deg. 16.45 min North
//			   12311.12,W   Longitude 123 deg. 11.12 min West
//			   000.5        Speed over ground, Knots
//			   054.7        Course Made Good, True
//			   191194       Date of fix  19 November 1994
//			   020.3,E      Magnetic variation 20.3 deg East
//			   *68          mandatory checksum
//
//
//	eg3. $GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70
//				  1    2    3    4    5     6    7    8      9     10  11 12
//
//
//		  1   220516     Time Stamp
//		  2   A          validity - A-ok, V-invalid
//		  3   5133.82    current Latitude
//		  4   N          North/South
//		  5   00042.24   current Longitude
//		  6   W          East/West
//		  7   173.8      Speed in knots
//		  8   231.8      True course
//		  9   130694     Date Stamp
//		  10  004.2      Variation
//		  11  W          East/West
//		  12  *70        checksum
//
//
//	eg4. $GPRMC,hhmmss.ss,A,ddmm.mmmm,a,dddmm.mmmm,a,x.x,x.x,ddmmyy,x.x,a*hh
//	1    = UTC of position fix
//	2    = Data status (V=navigation receiver warning)
//	3    = Latitude of fix
//	4    = N or S
//	5    = Longitude of fix
//	6    = E or W
//	7    = Speed over ground in knots
//	8    = Track made good in degrees True
//	9    = UT date
//	10   = Magnetic variation degrees (Easterly var. subtracts from true course)
//	11   = E or W
// NOTE: in some GPS units (GlobalTop) the 12th term is mode:
//	A = autonomous, D = differential, E = estimated
//	12 (or 13)   = Checksum
////////////////////////////////////////////////////////////
#if (GPSParser_INCLUDE_GPRMC)
void CGPSSentence_GPRMC::init(GPSSentenceData_T &_sentenceData)
{
	_sentenceData.GPRMC.m_utc = GPS_INVALID_DATA;			// hhmmss.ss = UTC of position
	_sentenceData.GPRMC.m_valid = false;					// validity - A-ok, V-invalid

	_sentenceData.GPRMC.m_lat = GPS_INVALID_DATA;			// llll.ll = latitude of position
	_sentenceData.GPRMC.m_north = false;					// a = N or S

	_sentenceData.GPRMC.m_lon = GPS_INVALID_DATA;			// yyyyy.yy = Longitude of position
	_sentenceData.GPRMC.m_east = false;						// a = E or W

	_sentenceData.GPRMC.m_speedKnots = GPS_INVALID_DATA;	// x.x = Speed over ground in knots
	_sentenceData.GPRMC.m_trackTrue = GPS_INVALID_DATA;		// x.x = Track made good in degrees True

	_sentenceData.GPRMC.m_date = GPS_INVALID_DATA;			// DDMMYY - Date of fix Year = YY
	_sentenceData.GPRMC.m_magDecl = GPS_INVALID_DATA;		// Magnetic variation from true North
	_sentenceData.GPRMC.m_magDeclEast = false;				// Magnet variation is East (true) or West (false)
}

bool CGPSSentence_GPRMC::processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData)
{
	switch(_termNumber)
	{
	case 1:		// hhmmss.ss = UTC of position
		_sentenceData.GPRMC.m_utc = GPSSentence_Convert_Double_Data(_term);
		break;

	case 2:		// validity - A-ok, V-invalid
		_sentenceData.GPRMC.m_valid = (_term[0] == 'A');
		break;

	case 3:		// ddmm.mmmm = latitude of position
		_sentenceData.GPRMC.m_lat = GPSSentence_Convert_LL_to_DD(_term);
		break;

	case 4:		// a = N or S
		if(_term[0] == 'N')
			_sentenceData.GPRMC.m_north = true;
		else
			_sentenceData.GPRMC.m_north = false;
		break;

	case 5:		// dddmm.mmmm = Longitude of position
		_sentenceData.GPRMC.m_lon = GPSSentence_Convert_LL_to_DD(_term);
		break;

	case 6:		// a = E or W
		if(_term[0] == 'E')
			_sentenceData.GPRMC.m_east = true;
		else
			_sentenceData.GPRMC.m_east = false;
		break;

	case 7:		// x.x = xx over ground in knots
		_sentenceData.GPRMC.m_speedKnots = GPSSentence_Convert_Double_Data(_term);
		break;

	case 8:		// x.x = Track made good in degrees True
		_sentenceData.GPRMC.m_trackTrue = GPSSentence_Convert_Double_Data(_term);
		break;

	case 9:		// DDMMYY - Date of fix Year = YY
		_sentenceData.GPRMC.m_date = GPSSentence_Convert_Long_Data(_term);
		break;

	case 10:	// Magnetic declination from true North
		_sentenceData.GPRMC.m_magDecl = GPSSentence_Convert_Double_Data(_term);
		break;

	case 11:	// Magnet variation is East (true) or West (false)
		if(_term[0] == 'E')
			_sentenceData.GPRMC.m_magDeclEast = true;
		else
			_sentenceData.GPRMC.m_magDeclEast = false;
		break;

	case 12:	// Special case for GlobalTop "Mode" parameter. See note above
		break;

	default:
		return false;
	}

	return true;
}

void CGPSSentence_GPRMC::transferData(CGPSParserData &_gpsParserData, GPSSentenceData_T &_sentenceData)
{
	// Time
	if(GPS_IS_VALID_DATA(_sentenceData.GPRMC.m_utc))
	{
		double time = _sentenceData.GPRMC.m_utc;
		double hour = (int)(time / 10000);
		time -= (hour * 10000);

		double minute = (int)(time / 100);
		time -= (minute * 100);

		double second = (int)time;

		_gpsParserData.m_time.m_hour = (int)hour;
		_gpsParserData.m_time.m_minute = (int)minute;
		_gpsParserData.m_time.m_second = (int)second;
	}
	else
	{
		_gpsParserData.m_time.m_hour = GPS_INVALID_DATA;
		_gpsParserData.m_time.m_minute = GPS_INVALID_DATA;
		_gpsParserData.m_time.m_second = GPS_INVALID_DATA;
	}

	// Valid fix
	_gpsParserData.m_GPSLocked = _sentenceData.GPRMC.m_valid;

	// Lat
	if(GPS_IS_VALID_DATA(_sentenceData.GPRMC.m_lat))
	{
		double lat = _sentenceData.GPRMC.m_lat *
					 (_sentenceData.GPRMC.m_north ? 1 : -1);
		_gpsParserData.m_position.m_lat = lat;
	}
	else
	{
		_gpsParserData.m_position.m_lat = GPS_INVALID_DATA;
	}

	// Lon
	if(GPS_IS_VALID_DATA(_sentenceData.GPRMC.m_lon))
	{
		double lat = _sentenceData.GPRMC.m_lon *
					 (_sentenceData.GPRMC.m_east ? 1 : -1);
		_gpsParserData.m_position.m_lon = lat;
	}
	else
	{
		_gpsParserData.m_position.m_lon = GPS_INVALID_DATA;
	}

	// Ground speed (provided as knots, but converted to kph for GPS data structure
	if(GPS_IS_VALID_DATA(_sentenceData.GPRMC.m_speedKnots))
	{
		_gpsParserData.m_groundSpeedKPH = _sentenceData.GPRMC.m_speedKnots * 1.852;
	}
	else
	{
		_gpsParserData.m_groundSpeedKPH = GPS_INVALID_DATA;
	}

	// Ground track
	GPSSentence_Transfer_Double_Data(_sentenceData.GPRMC.m_trackTrue, _gpsParserData.m_groundTrack);

	// Date
	if(GPS_IS_VALID_DATA(_sentenceData.GPRMC.m_date))
	{
		long date = _sentenceData.GPRMC.m_date;
		long day = (long)(date / 10000L);
		date -= (day * 10000L);

		long month = (long)(date / 100L);
		date -= (month * 100L);

		long year = (long)date;
		year += year > 80 ? 1900 : 2000;

		_gpsParserData.m_date.m_day = (int)day;
		_gpsParserData.m_date.m_month = (int)month;
		_gpsParserData.m_date.m_year = (int)year;
	}
	else
	{
		_gpsParserData.m_date.m_day = GPS_INVALID_DATA;
		_gpsParserData.m_date.m_month = GPS_INVALID_DATA;
		_gpsParserData.m_date.m_year = GPS_INVALID_DATA;
	}

	// Magnetic declination
	if(GPS_IS_VALID_DATA(_sentenceData.GPRMC.m_magDecl))
	{
		double magDecl = _sentenceData.GPRMC.m_magDecl *
						 (_sentenceData.GPRMC.m_magDeclEast ? 1 : -1);
		_gpsParserData.m_magDecl = magDecl;
	}
	else
	{
		_gpsParserData.m_magDecl = GPS_INVALID_DATA;
	}
}
#endif

////////////////////////////////////////////////////////////
//	$GPGSA
//
//	GPS DOP and active satellites
//
//	eg1. $GPGSA,A,3,,,,,,16,18,,22,24,,,3.6,2.1,2.2*3C
//	eg2. $GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35
//
//
//	1    = Mode:
//		   M=Manual, forced to operate in 2D or 3D
//		   A=Automatic, 3D/2D
//	2    = Mode:
//		   1=Fix not available
//		   2=2D
//		   3=3D
//	3-14 = IDs of SVs used in position fix (null for unused fields)
//	15   = PDOP
//	16   = HDOP
//	17   = VDOP
// 	18   = checksum
////////////////////////////////////////////////////////////
#if (GPSParser_INCLUDE_GPGSA)
void CGPSSentence_GPGSA::init(GPSSentenceData_T &_sentenceData)
{
	_sentenceData.GPGSA.m_modeMA = GPS_INVALID_DATA;
	_sentenceData.GPGSA.m_fixType = GPS_INVALID_DATA;

	for(int i = 0; i < GPS_MAX_SATS; ++i)
		_sentenceData.GPGSA.m_satIDs[i] = GPS_INVALID_DATA;

	_sentenceData.GPGSA.m_pDOP = GPS_INVALID_DATA;
	_sentenceData.GPGSA.m_hDOP = GPS_INVALID_DATA;
	_sentenceData.GPGSA.m_vDOP = GPS_INVALID_DATA;
}

bool CGPSSentence_GPGSA::processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData)
{
	switch(_termNumber)
	{
	case 1:
		if(_term[0] == 'M')
			_sentenceData.GPGSA.m_modeMA = 0;
		else
			_sentenceData.GPGSA.m_modeMA = 1;
		break;

	case 2:
		_sentenceData.GPGSA.m_fixType = GPSSentence_Convert_Int_Data(_term);
		break;

	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		_sentenceData.GPGSA.m_satIDs[_termNumber - 3] = GPSSentence_Convert_Int_Data(_term);
		break;

	case 15:
		_sentenceData.GPGSA.m_pDOP = GPSSentence_Convert_Double_Data(_term);
		break;

	case 16:
		_sentenceData.GPGSA.m_hDOP = GPSSentence_Convert_Double_Data(_term);
		break;

	case 17:
		_sentenceData.GPGSA.m_vDOP = GPSSentence_Convert_Double_Data(_term);
		break;

	default:
		return false;
	}

	return true;
}

void CGPSSentence_GPGSA::transferData(CGPSParserData &_gpsParserData, GPSSentenceData_T &_sentenceData)
{
	int GPGSA_SatIndex;
	int GPSData_SatIndex;

	// Manual or automatic fix mode
	if(GPS_IS_VALID_DATA(_sentenceData.GPGSA.m_modeMA))
		_gpsParserData.m_modeAutomatic = (_sentenceData.GPGSA.m_modeMA == 1);

	// Fix type: 1 = no fix, 2 = 2D, 3 = 3D
	GPSSentence_Transfer_Int_Data(_sentenceData.GPGSA.m_fixType, _gpsParserData.m_fixType);

	// Sat IDs in fix
	for(int clearIndex = 0; clearIndex < GPS_MAX_SATS; ++clearIndex)
		_gpsParserData.m_satInfo[clearIndex].m_inUse = false;

	// Flag the sats that are in use with this fix
	for(GPGSA_SatIndex = 0; GPGSA_SatIndex < GPS_MAX_SATS; ++GPGSA_SatIndex)
	{
		// Skip blank entries in the GPGSA data
		if(!GPS_IS_VALID_DATA(_sentenceData.GPGSA.m_satIDs[GPGSA_SatIndex]))
			continue;
		int inUsePRNID = _sentenceData.GPGSA.m_satIDs[GPGSA_SatIndex];

		for(GPSData_SatIndex = 0; GPSData_SatIndex < GPS_MAX_SATS; ++GPSData_SatIndex)
		{
			// Skip blank entries in the sats list
			if(!GPS_IS_VALID_DATA(_gpsParserData.m_satInfo[GPSData_SatIndex].m_satPRNID))
				continue;

			// If this sat is in use then flag it
			int satPRNID = _gpsParserData.m_satInfo[GPSData_SatIndex].m_satPRNID;
			if(satPRNID == inUsePRNID)
				_gpsParserData.m_satInfo[GPSData_SatIndex].m_inUse = true;
		}
	}

	// pDOP
	if(GPS_IS_VALID_DATA(_sentenceData.GPGSA.m_pDOP))
	{
		_gpsParserData.m_accuracyPositional = _sentenceData.GPGSA.m_pDOP * GPS_NMEA_DOP_FACTOR;

	}
	else
	{
		_gpsParserData.m_accuracyPositional = GPS_INVALID_DATA;
	}

	// hDOP
	if(GPS_IS_VALID_DATA(_sentenceData.GPGSA.m_hDOP))
	{
		_gpsParserData.m_accuracyHorizontal = _sentenceData.GPGSA.m_hDOP * GPS_NMEA_DOP_FACTOR;

	}
	else
	{
		_gpsParserData.m_accuracyHorizontal = GPS_INVALID_DATA;
	}

	// vDOP
	if(GPS_IS_VALID_DATA(_sentenceData.GPGSA.m_vDOP))
	{
		_gpsParserData.m_accuracyVertical = _sentenceData.GPGSA.m_vDOP * GPS_NMEA_DOP_FACTOR;

	}
	else
	{
		_gpsParserData.m_accuracyVertical = GPS_INVALID_DATA;
	}
}
#endif

////////////////////////////////////////////////////////////
//	$GPGSV
//
//	GPS Satellites in view
//
//	eg. $GPGSV,3,1,11,03,03,111,00,04,15,270,00,06,01,010,00,13,06,292,00*74
//		$GPGSV,3,2,11,14,25,170,00,16,57,208,39,18,67,296,40,19,40,246,00*74
//		$GPGSV,3,3,11,22,42,067,42,24,14,311,43,27,05,244,00,,,,*4D
//
//
//		$GPGSV,1,1,13,02,02,213,,03,-3,000,,11,00,121,,14,13,172,05*67
//
//
//	1    = Total number of messages of this type in this cycle
//	2    = Message number
//	3    = Total number of SVs in view
//	4    = SV PRN number
//	5    = Elevation in degrees, 90 maximum
//	6    = Azimuth, degrees from true north, 000 to 359
//	7    = SNR, 00-99 dB (null when not tracking)
//	8-11 = Information about second SV, same as field 4-7
//	12-15= Information about third SV, same as field 4-7
//	16-19= Information about fourth SV, same as field 4-7
////////////////////////////////////////////////////////////
#if (GPSParser_INCLUDE_GPGSV)
void CGPSSentence_GPGSV::init(GPSSentenceData_T &_sentenceData)
{
	_sentenceData.GPGSV.m_totalMessages = GPS_INVALID_DATA;	// Typically 3
	_sentenceData.GPGSV.m_messageNumber = GPS_INVALID_DATA;	// Could be 1,2, or 3

	_sentenceData.GPGSV.m_totalSatsInView = GPS_INVALID_DATA;	// Could be as high as 24

	_sentenceData.GPGSV.m_PRN_1 = GPS_INVALID_DATA;			// Sat ID number
	_sentenceData.GPGSV.m_elevation_1 = GPS_INVALID_DATA;	// Elevation in degrees
	_sentenceData.GPGSV.m_azimuth_1 = GPS_INVALID_DATA;		// Azimuth (direction) in degrees (True)
	_sentenceData.GPGSV.m_SNR_1 = GPS_INVALID_DATA;			// Signal quality (0-99) or null when not tracking

	_sentenceData.GPGSV.m_PRN_2 = GPS_INVALID_DATA;			// Sat ID number
	_sentenceData.GPGSV.m_elevation_2 = GPS_INVALID_DATA;	// Elevation in degrees
	_sentenceData.GPGSV.m_azimuth_2 = GPS_INVALID_DATA;		// Azimuth (direction) in degrees (True)
	_sentenceData.GPGSV.m_SNR_2 = GPS_INVALID_DATA;			// Signal quality (0-99) or null when not tracking

	_sentenceData.GPGSV.m_PRN_3 = GPS_INVALID_DATA;			// Sat ID number
	_sentenceData.GPGSV.m_elevation_3 = GPS_INVALID_DATA;	// Elevation in degrees
	_sentenceData.GPGSV.m_azimuth_3 = GPS_INVALID_DATA;		// Azimuth (direction) in degrees (True)
	_sentenceData.GPGSV.m_SNR_3 = GPS_INVALID_DATA;			// Signal quality (0-99) or null when not tracking

	_sentenceData.GPGSV.m_PRN_4 = GPS_INVALID_DATA;			// Sat ID number
	_sentenceData.GPGSV.m_elevation_4 = GPS_INVALID_DATA;	// Elevation in degrees
	_sentenceData.GPGSV.m_azimuth_4 = GPS_INVALID_DATA;		// Azimuth (direction) in degrees (True)
	_sentenceData.GPGSV.m_SNR_4 = GPS_INVALID_DATA;			// Signal quality (0-99) or null when not tracking
}

bool CGPSSentence_GPGSV::processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData)
{
	switch(_termNumber)
	{
	case 1:			// Total number of messages of this type in this cycle
		_sentenceData.GPGSV.m_totalMessages = GPSSentence_Convert_Int_Data(_term);
		break;

	case 2:			// Message number
		_sentenceData.GPGSV.m_messageNumber = GPSSentence_Convert_Int_Data(_term);
		break;

	case 3:			// Total number of SVs in view
		_sentenceData.GPGSV.m_totalSatsInView = GPSSentence_Convert_Int_Data(_term);
		break;

	case 4:			// SV PRN number
		_sentenceData.GPGSV.m_PRN_1 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 5:			// Elevation in degrees, 90 maximum
		_sentenceData.GPGSV.m_elevation_1 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 6:			// Azimuth, degrees from true north, 000 to 359
		_sentenceData.GPGSV.m_azimuth_1 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 7:			// SNR, 00-99 dB (null when not tracking)
		_sentenceData.GPGSV.m_SNR_1 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 8:			// SV PRN number
		_sentenceData.GPGSV.m_PRN_2 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 9:			// Elevation in degrees, 90 maximum
		_sentenceData.GPGSV.m_elevation_2 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 10:		// Azimuth, degrees from true north, 000 to 359
		_sentenceData.GPGSV.m_azimuth_2 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 11:		// SNR, 00-99 dB (null when not tracking)
		_sentenceData.GPGSV.m_SNR_2 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 12:			// SV PRN number
		_sentenceData.GPGSV.m_PRN_3 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 13:			// Elevation in degrees, 90 maximum
		_sentenceData.GPGSV.m_elevation_3 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 14:		// Azimuth, degrees from true north, 000 to 359
		_sentenceData.GPGSV.m_azimuth_3 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 15:		// SNR, 00-99 dB (null when not tracking)
		_sentenceData.GPGSV.m_SNR_3 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 16:			// SV PRN number
		_sentenceData.GPGSV.m_PRN_4 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 17:			// Elevation in degrees, 90 maximum
		_sentenceData.GPGSV.m_elevation_4 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 18:		// Azimuth, degrees from true north, 000 to 359
		_sentenceData.GPGSV.m_azimuth_4 = GPSSentence_Convert_Int_Data(_term);
		break;

	case 19:		// SNR, 00-99 dB (null when not tracking)
		_sentenceData.GPGSV.m_SNR_4 = GPSSentence_Convert_Int_Data(_term);
		break;

	default:
		return false;
	}

	return true;
}

void CGPSSentence_GPGSV::transferData(CGPSParserData &_gpsParserData, GPSSentenceData_T &_sentenceData)
{
	if(GPS_IS_VALID_DATA(_sentenceData.GPGSV.m_messageNumber) &&
			(_sentenceData.GPGSV.m_messageNumber > 0))
	{
		int satBase = (_sentenceData.GPGSV.m_messageNumber - 1) * 4;

		// Watch out for GPSs that provide info on more sats than we can handle
		if((satBase + 0) >= GPS_MAX_SATS) return;
		_gpsParserData.m_satInfo[satBase + 0].m_satPRNID = _sentenceData.GPGSV.m_PRN_1;
		_gpsParserData.m_satInfo[satBase + 0].m_elevation = _sentenceData.GPGSV.m_elevation_1;
		_gpsParserData.m_satInfo[satBase + 0].m_azimuth = _sentenceData.GPGSV.m_azimuth_1;
		_gpsParserData.m_satInfo[satBase + 0].m_signalStrength = _sentenceData.GPGSV.m_SNR_1;

		if((satBase + 1) >= GPS_MAX_SATS) return;
		_gpsParserData.m_satInfo[satBase + 1].m_satPRNID = _sentenceData.GPGSV.m_PRN_2;
		_gpsParserData.m_satInfo[satBase + 1].m_elevation = _sentenceData.GPGSV.m_elevation_2;
		_gpsParserData.m_satInfo[satBase + 1].m_azimuth = _sentenceData.GPGSV.m_azimuth_2;
		_gpsParserData.m_satInfo[satBase + 1].m_signalStrength = _sentenceData.GPGSV.m_SNR_2;

		if((satBase + 2) >= GPS_MAX_SATS) return;
		_gpsParserData.m_satInfo[satBase + 2].m_satPRNID = _sentenceData.GPGSV.m_PRN_3;
		_gpsParserData.m_satInfo[satBase + 2].m_elevation = _sentenceData.GPGSV.m_elevation_3;
		_gpsParserData.m_satInfo[satBase + 2].m_azimuth = _sentenceData.GPGSV.m_azimuth_3;
		_gpsParserData.m_satInfo[satBase + 2].m_signalStrength = _sentenceData.GPGSV.m_SNR_3;

		if((satBase + 3) >= GPS_MAX_SATS) return;
		_gpsParserData.m_satInfo[satBase + 3].m_satPRNID = _sentenceData.GPGSV.m_PRN_4;
		_gpsParserData.m_satInfo[satBase + 3].m_elevation = _sentenceData.GPGSV.m_elevation_3;
		_gpsParserData.m_satInfo[satBase + 3].m_azimuth = _sentenceData.GPGSV.m_azimuth_4;
		_gpsParserData.m_satInfo[satBase + 3].m_signalStrength = _sentenceData.GPGSV.m_SNR_4;
	}
}
#endif

////////////////////////////////////////////////////////////
//	$GPVTG
//
//	Track Made Good and Ground Speed.
//
//	eg1. $GPVTG,360.0,T,348.7,M,000.0,N,000.0,K*43
//	eg2. $GPVTG,054.7,T,034.4,M,005.5,N,010.2,K
//
//
//			   054.7,T      True track made good
//			   034.4,M      Magnetic track made good
//			   005.5,N      Ground speed, knots
//			   010.2,K      Ground speed, Kilometers per hour
//
//
//	eg3. $GPVTG,t,T,,,s.ss,N,s.ss,K*hh
//	1    = Track made good
//	2    = Fixed text 'T' indicates that track made good is relative to true north
//	3    = not used
//	4    = not used
//	5    = Speed over ground in knots
//	6    = Fixed text 'N' indicates that speed over ground in in knots
//	7    = Speed over ground in kilometers/hour
//	8    = Fixed text 'K' indicates that speed over ground is in kilometers/hour
// NOTE: in some GPS units (GlobalTop) the 9th term is mode:
//	A = autonomous, D = differential, E = estimated
//	9    = Checksum
//
//	The actual track made good and speed relative to the ground.
//
//	$--VTG,x.x,T,x.x,M,x.x,N,x.x,K
//	x.x,T = Track, degrees True
//	x.x,M = Track, degrees Magnetic
//	x.x,N = Speed, knots
//	x.x,K = Speed, Km/hr
////////////////////////////////////////////////////////////
#if (GPSParser_INCLUDE_GPVTG)
void CGPSSentence_GPVTG::init(GPSSentenceData_T &_sentenceData)
{
	_sentenceData.GPVTG.m_trackTrue = GPS_INVALID_DATA;		// x.x = Track made good in degrees True
	_sentenceData.GPVTG.m_trackMag = GPS_INVALID_DATA;		// Track (magnetic)
	_sentenceData.GPVTG.m_speedKnots = GPS_INVALID_DATA;	// x.x = Speed over ground in knots
	_sentenceData.GPVTG.m_speedKPH = GPS_INVALID_DATA;		// x.x = Speed over ground in knots
}

bool CGPSSentence_GPVTG::processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData)
{
	switch(_termNumber)
	{
	case 1:			// Track made good True
		_sentenceData.GPVTG.m_trackTrue = GPSSentence_Convert_Double_Data(_term);
		break;

	case 3:	// Track made good Magnetic
		_sentenceData.GPVTG.m_trackMag = GPSSentence_Convert_Double_Data(_term);
		break;

	case 5:
		_sentenceData.GPVTG.m_speedKnots = GPSSentence_Convert_Double_Data(_term);
		break;

		break;

	case 7:
		_sentenceData.GPVTG.m_speedKPH = GPSSentence_Convert_Double_Data(_term);
		break;

	case 2:	// Fixed 'T'
	case 4:	// Fixed 'M'
	case 6:	// Fixed 'N'
	case 8:	// Fixed 'K'
	case 9:	// Special case for GlobalTop "Mode" parameter. See note above
		break;

	default:
		return false;
	}

	return true;
}

void CGPSSentence_GPVTG::transferData(CGPSParserData &_gpsParserData, GPSSentenceData_T &_sentenceData)
{
	// True ground track
	GPSSentence_Transfer_Double_Data(_sentenceData.GPVTG.m_trackTrue, _gpsParserData.m_groundTrack);

	// We only use speed i KPH, Knots are (IMHO) useless
	GPSSentence_Transfer_Double_Data(_sentenceData.GPVTG.m_speedKPH, _gpsParserData.m_groundSpeedKPH);

	// Magnetic Declination
	if(_sentenceData.GPVTG.m_trackTrue != GPS_INVALID_DATA && _sentenceData.GPVTG.m_trackMag != GPS_INVALID_DATA)
	{
		_gpsParserData.m_magDecl = _sentenceData.GPVTG.m_trackTrue - _sentenceData.GPVTG.m_trackMag;
		if(_gpsParserData.m_magDecl > 180.)
			_gpsParserData.m_magDecl = _gpsParserData.m_magDecl - 360.;
	}
}
#endif
