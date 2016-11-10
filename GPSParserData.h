////////////////////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
//
// GPS Parser Common data
////////////////////////////////////////////////////////////
#ifndef GPSPARSER_DATA_H
#define GPSPARSER_DATA_H

//////////////////////////////////////////////
// Embedded common data types
#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
class CGPSParser_Time
{
	public:
	int m_hour, m_minute, m_second;	// (Hour: 0-23, Minutes & Seconds: 0-59)

	CGPSParser_Time()
	{
		m_hour = m_minute = m_second = GPS_INVALID_DATA;
	}

	CGPSParser_Time &operator=(const CGPSParser_Time &_time)
	{
		m_hour = _time.m_hour;
		m_minute = _time.m_minute;
		m_second = _time.m_second;

		return *this;
	}

	bool operator==(const CGPSParser_Time &_time)
	{
		if(m_hour != _time.m_hour) return false;
		if(m_minute != _time.m_minute) return false;
		if(m_second != _time.m_second) return false;
		return true;
	}

	bool operator!=(const CGPSParser_Time &_time)
	{
		return !(*this == _time);
	}
};
#endif

#if (GPSParser_INCLUDE_GPRMC)
class CGPSParser_Date
{
public:
	int m_year, m_month, m_day;	// Year in CE, Month: 1-12, Day: 1-31)

	CGPSParser_Date()
	{
		m_year = m_month = m_day = GPS_INVALID_DATA;
	}

	CGPSParser_Date &operator=(const CGPSParser_Date &_date)
	{
		m_year = _date.m_year;
		m_month = _date.m_month;
		m_day = _date.m_day;

		return *this;
	}

	bool operator==(const CGPSParser_Date &_date)
	{
		if(m_year != _date.m_year) return false;
		if(m_month != _date.m_month) return false;
		if(m_day != _date.m_day) return false;
		return true;
	}

	bool operator!=(const CGPSParser_Date &_date)
	{
		return !(*this == _date);
	}
};
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
class CGPSParser_Position
{
	public:
	double m_lat, m_lon;

	CGPSParser_Position()
	{
		m_lat = m_lon = GPS_INVALID_DATA;
	}

	CGPSParser_Position &operator=(const CGPSParser_Position &_pos)
	{
		m_lat = _pos.m_lat;
		m_lon = _pos.m_lon;
		return *this;
	}

	bool operator==(const CGPSParser_Position &_pos)
	{
		if(m_lat != _pos.m_lat) return false;
		if(m_lon != _pos.m_lon) return false;
		return true;
	}

	bool operator!=(const CGPSParser_Position &_pos)
	{
		return !(*this == _pos);
	}
};
#endif

#if (GPSParser_INCLUDE_GPGSV || GPSParser_INCLUDE_GPGSA)
class CGPSParser_SatInfo
{
	public:
	int m_satPRNID;		// Sat ID number
	bool m_inUse;			// True if used in fix
	int m_elevation;		// 0 - 90 degrees
	int m_azimuth;			// 0 - 359 degrees
	int m_signalStrength;	// 0 - 99

	CGPSParser_SatInfo()
	{
		clear();
	}

	void clear()
	{
		m_satPRNID = GPS_INVALID_DATA;
		m_inUse = false;
		m_elevation = GPS_INVALID_DATA;
		m_azimuth = GPS_INVALID_DATA;
		m_signalStrength = GPS_INVALID_DATA;
	}

	CGPSParser_SatInfo &operator=(const CGPSParser_SatInfo &_satInfo)
	{
		m_satPRNID = _satInfo.m_satPRNID;
		m_inUse = _satInfo.m_inUse;
		m_elevation = _satInfo.m_elevation;
		m_azimuth = _satInfo.m_azimuth;
		m_signalStrength = _satInfo.m_signalStrength;

		return *this;
	}


	bool operator==(const CGPSParser_SatInfo &_satInfo)
	{
		if(m_satPRNID != _satInfo.m_satPRNID) return false;
		if(m_inUse != _satInfo.m_inUse) return false;
		if(m_elevation != _satInfo.m_elevation) return false;
		if(m_azimuth != _satInfo.m_azimuth) return false;
		if(m_signalStrength != _satInfo.m_signalStrength) return false;

		return true;
	}

	bool operator!=(const CGPSParser_SatInfo &_satInfo)
	{
		return !(*this == _satInfo);
	}
};
#endif

class CGPSParser_Data
{
	public:
#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
	bool m_GPSLocked;					// GPS is locked (GPGGA, GPRMC)
#endif

#if (GPSParser_INCLUDE_GPGGA)
	int m_fixQuality;					// Type of fix (0=none, 1=GPS, 2=Diff) (GPGGA)
	int m_nSatellites;					// number of sats providing lock (GPGGA)
#endif

#if (GPSParser_INCLUDE_GPGSA)
	bool m_modeAutomatic;				// Fix mode (Manual or Automatic selection of 2D or 3D fix... see fix type next) (GPGSA)
	int m_fixType;						// // 1 = no fix, 2 = 2D, 3 = 3D (GPGSA)
#endif

#if (GPSParser_INCLUDE_GPGGA)
	int m_diffFixAge;					// Age of last differential update in seconds (GPGGA)
	int m_diffStationID;				// Differential station ID (GPGGA)
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
	CGPSParser_Time m_time;				// last reported time (GPGGA, GPRMC)
#endif

#if (GPSParser_INCLUDE_GPRMC)
	CGPSParser_Date m_date;				// last reported date (GPRMC)
#endif

#if (GPSParser_INCLUDE_GPGGA)
	double m_altitude;					// Altitude of antenna above mean sea level in meters (GPGGA)
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
	CGPSParser_Position m_position;		// last reported position (GPGGA, GPRMC)
#endif

#if (GPSParser_INCLUDE_GPRMC || GPSParser_INCLUDE_GPVTG)
	double m_groundSpeedKPH;			// In kph (GPRMC, GPVTG)
	double m_groundTrack;				// In degrees (true course) (GPRMC, GPVTG)
	double m_magDecl;					// Magnetic declination (GPRMC, GPVTG)
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPGSA)
	double m_accuracyHorizontal;		// horizontal Accuracy (+/-) (meters) (GPGGA, GPGSA)
#endif

#if (GPSParser_INCLUDE_GPGSA)
	double m_accuracyVertical;			// vertical Accuracy (+/-) (meters) (GPGSA)
	double m_accuracyPositional;		// complete vertical and horizontal accuracy (+/-) (meters) (GPGSA)
#endif

#if (GPSParser_INCLUDE_GPGSV || GPSParser_INCLUDE_GPGSA)
	CGPSParser_SatInfo m_satInfo[GPS_MAX_SATS];	// Info about all sats in view (GPGSV AND GPGSA)
#endif

	CGPSParser_Data()
	{
		clear();
	}

	void clear()
	{
#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		m_GPSLocked = false;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		m_fixQuality = 0;
		m_nSatellites = GPS_INVALID_DATA;
#endif

#if (GPSParser_INCLUDE_GPGSA)
		m_modeAutomatic = false;
		m_fixType = 0;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		m_diffFixAge = GPS_INVALID_DATA;
		m_diffStationID = GPS_INVALID_DATA;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		CGPSParser_Time t;
		m_time = t;
#endif

#if (GPSParser_INCLUDE_GPRMC)
		CGPSParser_Date d;
		m_date = d;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		m_altitude = GPS_INVALID_DATA;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		CGPSParser_Position p;
		m_position = p;
#endif

#if (GPSParser_INCLUDE_GPRMC || GPSParser_INCLUDE_GPVTG)
		m_groundSpeedKPH = GPS_INVALID_DATA;
		m_groundTrack = GPS_INVALID_DATA;
		m_magDecl = GPS_INVALID_DATA;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPGSA)
		m_accuracyHorizontal = GPS_INVALID_DATA;
#endif

#if (GPSParser_INCLUDE_GPGSA)
		m_accuracyVertical = GPS_INVALID_DATA;
		m_accuracyPositional = GPS_INVALID_DATA;
#endif

#if (GPSParser_INCLUDE_GPGSV || GPSParser_INCLUDE_GPGSA)
		for(int iSatIndex = 0; iSatIndex < GPS_MAX_SATS; ++iSatIndex)
			m_satInfo[iSatIndex].clear();
#endif
	}

	CGPSParser_Data &operator=(const CGPSParser_Data &_pd)
	{
#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		m_GPSLocked = _pd.m_GPSLocked;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		m_fixQuality = _pd.m_fixQuality;
		m_nSatellites = _pd.m_nSatellites;
#endif

#if (GPSParser_INCLUDE_GPGSA)
		m_modeAutomatic = _pd.m_modeAutomatic;
		m_fixType = _pd.m_fixType;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		m_diffFixAge = _pd.m_diffFixAge;
		m_diffStationID = _pd.m_diffStationID;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		m_time = _pd.m_time;
#endif

#if (GPSParser_INCLUDE_GPRMC)
		m_date = _pd.m_date;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		m_altitude = _pd.m_altitude;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		m_position = _pd.m_position;
#endif

#if (GPSParser_INCLUDE_GPRMC || GPSParser_INCLUDE_GPVTG)
		m_groundSpeedKPH = _pd.m_groundSpeedKPH;
		m_groundTrack = _pd.m_groundTrack;
		m_magDecl = _pd.m_magDecl;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPGSA)
		m_accuracyHorizontal = _pd.m_accuracyHorizontal;
#endif

#if (GPSParser_INCLUDE_GPGSA)
		m_accuracyVertical = _pd.m_accuracyVertical;
		m_accuracyPositional = _pd.m_accuracyPositional;
#endif

#if (GPSParser_INCLUDE_GPGSV || GPSParser_INCLUDE_GPGSA)
		for(int iSatIndex = 0; iSatIndex < GPS_MAX_SATS; ++iSatIndex)
			m_satInfo[iSatIndex] = _pd.m_satInfo[iSatIndex];
#endif

		return *this;
	}

	bool operator==(const CGPSParser_Data &_pd)
	{
#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		if(m_GPSLocked != _pd.m_GPSLocked) return false;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		if(m_fixQuality != _pd.m_fixQuality) return false;
		if(m_nSatellites != _pd.m_nSatellites) return false;
#endif

#if (GPSParser_INCLUDE_GPGSA)
		if(m_modeAutomatic != _pd.m_modeAutomatic) return false;
		if(m_fixType != _pd.m_fixType) return false;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		if(m_diffFixAge != _pd.m_diffFixAge) return false;
		if(m_diffStationID != _pd.m_diffStationID) return false;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		if(m_time != _pd.m_time) return false;
#endif

#if (GPSParser_INCLUDE_GPRMC)
		if(m_date != _pd.m_date) return false;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPRMC)
		if(m_position != _pd.m_position) return false;
#endif

#if (GPSParser_INCLUDE_GPGGA)
		if(m_altitude != _pd.m_altitude) return false;
#endif

#if (GPSParser_INCLUDE_GPRMC || GPSParser_INCLUDE_GPVTG)
		if(m_groundSpeedKPH != _pd.m_groundSpeedKPH) return false;
		if(m_groundTrack != _pd.m_groundTrack) return false;
		if(m_magDecl != _pd.m_magDecl) return false;
#endif

#if (GPSParser_INCLUDE_GPGGA || GPSParser_INCLUDE_GPGSA)
		if(m_accuracyHorizontal != _pd.m_accuracyHorizontal) return false;
#endif

#if (GPSParser_INCLUDE_GPGSA)
		if(m_accuracyVertical != _pd.m_accuracyVertical) return false;
		if(m_accuracyPositional != _pd.m_accuracyPositional) return false;
#endif

#if (GPSParser_INCLUDE_GPGSV || GPSParser_INCLUDE_GPGSA)
		for(int iSatIndex = 0; iSatIndex < GPS_MAX_SATS; ++iSatIndex)
			if(m_satInfo[iSatIndex] != _pd.m_satInfo[iSatIndex]) return false;
#endif

		return true;
	}

	bool operator!=(const CGPSParser_Data &_pd)
	{
		return !(*this == _pd);
	}
};

#endif
