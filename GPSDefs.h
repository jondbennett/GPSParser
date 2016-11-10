
#ifndef GPSDefs_h
#define GPSDefs_h

///////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
// These are GPS-related values that are
// used throughout the GPS library, so
// it makes sense to have them in a common place
///////////////////////////////////////////////

#define GPS_MAX_SATS			(16)	// Depends on GPS, but 12 is common. Should be a multiple of 4

// Things related to GPS location and distance
#define GPS_INVALID_DATA		(9966)
#define GPS_IS_VALID_DATA(x)	( ((x > 9965) && (x < 9967))? false : true )

// Useful for certain calculations
#define GPS_PI 						(3.1415926535897932384626433832795)

//#define GPS_PI180					((GPSPI) / 180.)
#define GPS_PI180					(0.017453292519943295769236907684886)

#define GPS_D2R(v)					((v) * (GPS_PI180))
#define	GPS_R2D(v)					((v) / (GPS_PI180))

#define GPS_EARTHRADIUS_KM         (6371.009)						// Earth's mean radius in km
#define GPS_EARTHRADIUS_M          ((GPS_EARTHRADIUS_KM) * 1000.)	// Earth's mean radius in m

// From the NMEA library to convert DOP to meters
#define GPS_NMEA_DOP_FACTOR			(5.)							// Factor for DOP to meters

// Select which sentences to include
#define GPSParser_INCLUDE_GPGGA		(1)	// Global Positioning System Fix Data (Lat, lon, time, fix quality, num sats, hdop, altitude)
#define GPSParser_INCLUDE_GPRMC		(1)	// Recommended minimum specific GPS/Transit data (time, lat, lon, speed, course, date)
#define GPSParser_INCLUDE_GPGSA		(1)	// GPS DOP and active satellites (list of sats in position fix, pdop, hdop, vdop)
#define GPSParser_INCLUDE_GPGSV		(1)	// GPS Satellites in view (list of sats in view, and their asimuth and elevation)
#define GPSParser_INCLUDE_GPVTG		(1)	// Track made good and ground speed

#endif
