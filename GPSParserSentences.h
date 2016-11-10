////////////////////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
//
// GPS Parser sentence objects
////////////////////////////////////////////////////////////
#ifndef GPSPARSER_SENTENCES
#define GPSPARSER_SENTENCES

// Sentence types
typedef enum GPSSentenceE
{
	GPSSentence_T_Unknown = 0,
#if (GPSParser_INCLUDE_GPGGA)
	GPSSentence_T_GPGGA,	// Global Positioning System Fix Data
#endif

#if (GPSParser_INCLUDE_GPRMC)
	GPSSentence_T_GPRMC,	// Recommended minimum specific GPS/Transit data
#endif

#if (GPSParser_INCLUDE_GPGSA)
	GPSSentence_T_GPGSA,	// GPS DOP and active satellites
#endif

#if (GPSParser_INCLUDE_GPGSV)
	GPSSentence_T_GPGSV,	// GPS Satellites in view
#endif

#if (GPSParser_INCLUDE_GPVTG)
	GPSSentence_T_GPVTG,	// Track Made Good and Ground Speed.
#endif

} GPSSentence_T;


// Data structure used to accumulate data
// as sentences are parsed
typedef union GPSSentenceData_U
{
#if (GPSParser_INCLUDE_GPGGA)
	struct
	{
		double m_utc;						// hhmmss.ss = UTC of position

		double m_lat;						// ddmm.mmmm = latitude of position
		bool m_north;						// a = N or S

		double m_lon;						// dddmm.mmmm = Longitude of position
		bool m_east;						// a = E or W

		int m_fixQuality;					// x = GPS Quality indicator (0=no fix, 1=GPS fix, 2=Dif. GPS fix)
		int m_nSats;						// xx = number of satellites in use
		double m_hDOP;						// x.x = horizontal dilution of precision

		double m_altitude;					// x.x = Antenna altitude above mean-sea-level

		double m_heightAboveWGS84;			// x.x = WGS84 geoidal separation
		int m_timeSinceLastDGPSUpdate;		// x.x = Age of Differential GPS data (seconds)

		int m_dgpsStationID;				// xxxx = Differential reference station ID
	} GPGGA;
#endif

#if (GPSParser_INCLUDE_GPRMC)
	struct
	{
		double m_utc;						// hhmmss.ss = UTC of position
		bool m_valid;						// validity - A-ok, V-invalid

		double m_lat;						// ddmm.mmmm = latitude of position
		bool m_north;						// a = N or S

		double m_lon;						// dddmm.mmmm = Longitude of position
		bool m_east;						// a = E or W

		double m_speedKnots;				// x.x = Speed over ground in knots
		double m_trackTrue;					// x.x = Track made good in degrees True

		long m_date;						// DDMMYY - Date of fix Year = YY

		double m_magDecl;					// Magnetic declination from true North
		bool m_magDeclEast;					// Magnet declination is East (true) or West (false)
	} GPRMC;
#endif

#if (GPSParser_INCLUDE_GPGSA)
	struct
	{
		int m_modeMA;						// 0 = manual, 1 = automatic (auto select of 2D or 3D)
		int m_fixType;						// 1 = no fix, 2 = 2D, 3 = 3D
		int m_satIDs[GPS_MAX_SATS];			// IDs of sats used for fix
		double m_pDOP;
		double m_hDOP;
		double m_vDOP;
	} GPGSA;
#endif

#if (GPSParser_INCLUDE_GPGSV)
	struct
	{
		int m_totalMessages;	// Typically 3
		int m_messageNumber;	// Could be 1,2, or 3

		int m_totalSatsInView;	// Could be as high as 12

		int m_PRN_1;			// Sat ID number
		int m_elevation_1;		// Elevation in degrees
		int m_azimuth_1;		// Azimuth (direction) in degrees (True)
		int m_SNR_1;			// Signal quality (0-99) or null when not tracking

		int m_PRN_2;			// Sat ID number
		int m_elevation_2;		// Elevation in degrees
		int m_azimuth_2;		// Azimuth (direction) in degrees (True)
		int m_SNR_2;			// Signal quality (0-99) or null when not tracking

		int m_PRN_3;			// Sat ID number
		int m_elevation_3;		// Elevation in degrees
		int m_azimuth_3;		// Azimuth (direction) in degrees (True)
		int m_SNR_3;			// Signal quality (0-99) or null when not tracking

		int m_PRN_4;			// Sat ID number
		int m_elevation_4;		// Elevation in degrees
		int m_azimuth_4;		// Azimuth (direction) in degrees (True)
		int m_SNR_4;			// Signal quality (0-99) or null when not tracking
	} GPGSV;
#endif

#if (GPSParser_INCLUDE_GPVTG)
	struct
	{
		double m_trackTrue;						// x.x = Track made good in degrees True
		double m_trackMag;						// Track (magnetic)
		double m_speedKnots;					// x.x = Speed over ground in knots
		double m_speedKPH;						// x.x = Speed over ground in kph
	} GPVTG;
#endif
} GPSSentenceData_T;

#if GPSParser_INCLUDE_GPGGA
class CGPSSentence_GPGGA
{
	public:

	static void init(GPSSentenceData_T &_sentenceData);
	static bool processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData);
	static void transferData(CGPSParser_Data &_gpsParserData, GPSSentenceData_T &_sentenceData);
};
#endif

#if (GPSParser_INCLUDE_GPRMC)
class CGPSSentence_GPRMC
{
	public:

	static void init(GPSSentenceData_T &_sentenceData);
	static bool processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData);
	static void transferData(CGPSParser_Data &_gpsParserData, GPSSentenceData_T &_sentenceData);
};
#endif

#if (GPSParser_INCLUDE_GPGSA)
class CGPSSentence_GPGSA
{
	public:

	static void init(GPSSentenceData_T &_sentenceData);
	static bool processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData);
	static void transferData(CGPSParser_Data &_gpsParserData, GPSSentenceData_T &_sentenceData);
};
#endif

#if (GPSParser_INCLUDE_GPGSV)
class CGPSSentence_GPGSV
{
	public:

	static void init(GPSSentenceData_T &_sentenceData);
	static bool processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData);
	static void transferData(CGPSParser_Data &_gpsParserData, GPSSentenceData_T &_sentenceData);
};
#endif
#if (GPSParser_INCLUDE_GPVTG)
class CGPSSentence_GPVTG
{
	public:

	static void init(GPSSentenceData_T &_sentenceData);
	static bool processTerm(const char _term[], int _termNumber, GPSSentenceData_T &_sentenceData);
	static void transferData(CGPSParser_Data &_gpsParserData, GPSSentenceData_T &_sentenceData);
};
#endif


#endif

