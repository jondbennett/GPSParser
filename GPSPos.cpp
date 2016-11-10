/////////////////////////////////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
//
// Basic spherical trig to calculate bearings and range.
/////////////////////////////////////////////////////////////////////////
#include <math.h>

#include "GPSParser.h"

#include "GPSPos.h"

#if defined(_MSC_VER)
# define GPS_POSIX(x)  _##x
#else
# define GPS_POSIX(x)  x
#endif

// 'Structors
CGPSPos::CGPSPos()
{
	m_lat = m_lon = GPS_INVALID_DATA;
}

CGPSPos::CGPSPos(double _dLat, double _dLon)
{
	m_lat = _dLat;
	m_lon = _dLon;
}

CGPSPos::CGPSPos(const CGPSPos &_pos)
{
	m_lat = _pos.getLat();
	m_lon = _pos.getLon();
}

CGPSPos &CGPSPos::operator=(const CGPSPos &_pos)
{
	m_lat = _pos.getLat();
	m_lon = _pos.getLon();
	return *this;
}

bool CGPSPos::operator==(const CGPSPos &_pos)
{
	if(getLat() == _pos.getLat() &&
		getLon() == _pos.getLon())
		return true;

	return false;
}

void CGPSPos::setPosition(double _dLat, double _dLon)
{
	m_lat = _dLat;
	m_lon = _dLon;
}

void CGPSPos::getPosition(double *_pdLat, double *_pdLon)
{
	if(_pdLat) *_pdLat = m_lat;
	if(_pdLon) *_pdLon = m_lon;
}

void CGPSPos::clear()
{
	m_lat = m_lon = GPS_INVALID_DATA;
}


// The guts of this method were shamelessly "borrowed" from
// the nmea library (gmath.c) nmea_distance function.
// If this is broken it is my fault.
// Original credit:
/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: gmath.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */
double distance_between_NMEA(double _dLat1, double _dLon1, double _dLat2, double _dLon2)
{


double dHere_lat, dHere_lon, dThere_lat, dThere_lon;
double distance;

	dHere_lat = GPS_D2R(_dLat1);
	dHere_lon = GPS_D2R(_dLon1);

	dThere_lat = GPS_D2R( _dLat2);
	dThere_lon = GPS_D2R(_dLon2);

	distance = ((double)GPS_EARTHRADIUS_M) * acos(
			sin(dThere_lat) * sin(dHere_lat) +
			cos(dThere_lat) * cos(dHere_lat) * cos(dThere_lon - dHere_lon)
			);

	return distance;
}

#define DEG_TO_RAD 0.017453292519943295769236907684886
#define radians(deg) ((deg)*DEG_TO_RAD)
#define sq(x) ((x)*(x))
double distance_between_TinyGPS(float lat1, float long1, float lat2, float long2)
{
  // returns distance in meters between two positions, both specified
  // as signed decimal-degrees latitude and longitude. Uses great-circle
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  float delta = radians(long1-long2);
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * GPS_EARTHRADIUS_M;
  }

// Calculations
double CGPSPos::distanceToInMeters(CGPSPos _pos)
{
	double dHere_lat = getLat();
	double dHere_lon = getLon();

	double dThere_lat = _pos.getLat();
	double dThere_lon = _pos.getLon();

	return distance_between_TinyGPS(dHere_lat, dHere_lon, dThere_lat, dThere_lon);
//	return distance_between_NMEA(dHere_lat, dHere_lon, dThere_lat, dThere_lon);
}

// Using Haversine
double CGPSPos::trueHeadingTo(CGPSPos _pos)
{
double dHere_lat, dHere_lon, dThere_lat, dThere_lon;
double x, y, londiff, heading;

	dHere_lat = GPS_D2R(getLat());
	dHere_lon = GPS_D2R(getLon());

	dThere_lat = GPS_D2R(_pos.getLat());
	dThere_lon = GPS_D2R(_pos.getLon());

    londiff = dThere_lon - dHere_lon;

	y = (sin(londiff)*cos(dThere_lat));
	x = ((cos(dHere_lat)*sin(dThere_lat))-(sin(dHere_lat)*cos(dThere_lat)*cos(londiff)));

    heading = atan2(y, x) ;
    heading = GPS_R2D(heading);

    if(heading < 0.) heading += 360.;

	return heading;
}

bool CGPSPos::displaceByMeters(double _dRadial, double _dDistance)
{
// The guts of this method were shamelessly "borrowed" from
// the nmea library (gmath.c) nmea_move_horz function.
// If this is broken it is my fault.
// Original credit:
double dHere_lat, dHere_lon, dThere_lat, dThere_lon;
double dAzimuth;

	dHere_lat = GPS_D2R(getLat());
	dHere_lon = GPS_D2R(getLon());

    _dDistance /= GPS_EARTHRADIUS_M;
    dAzimuth = GPS_D2R(_dRadial);

    dThere_lat = asin(
        sin(dHere_lat) * cos(_dDistance) + cos(dHere_lat) * sin(_dDistance) * cos(dAzimuth));
    dThere_lon = dHere_lon + atan2(
        sin(dAzimuth) * sin(_dDistance) * cos(dHere_lat), cos(_dDistance) - sin(dHere_lat) * sin(dThere_lat));

    if(GPS_POSIX(isnan)(dThere_lat) || GPS_POSIX(isnan)(dThere_lon))
    {
        dThere_lat = GPS_INVALID_DATA;
        dThere_lon = GPS_INVALID_DATA;
        return false;
    }

	setPosition(GPS_R2D(dThere_lat),
				GPS_R2D(dThere_lon));
	return true;
}

/////////////////////////////////////////////////////////////////////////
// These are the original functions from the nmea library
// Original credit:
/*
 *
 * NMEA library
 * URL: http://nmea.sourceforge.net
 * Author: Tim (xtimor@gmail.com)
 * Licence: http://www.gnu.org/licenses/lgpl.html
 * $Id: gmath.c 17 2008-03-11 11:56:11Z xtimor $
 *
 */

# if 0
/**
 * Position data in fractional degrees or radians
 */
typedef struct _nmeaPOS
{
    double lat;         /**< Latitude */
    double lon;         /**< Longitude */

} nmeaPOS;

/**
 * \fn nmea_degree2radian
 * \brief Convert degree to radian
 */
double nmea_degree2radian(double val)
{ return (val * NMEA_PI180); }

/**
 * \fn nmea_radian2degree
 * \brief Convert radian to degree
 */
double nmea_radian2degree(double val)
{ return (val / NMEA_PI180); }

/**
 * \brief Calculate distance between two points
 * \return Distance in meters
 */
double nmea_distance(
        const nmeaPOS *from_pos,    /**< From position in radians */
        const nmeaPOS *to_pos       /**< To position in radians */
        )
{
    double dist = ((double)NMEA_EARTHRADIUS_M) * acos(
        sin(to_pos->lat) * sin(from_pos->lat) +
        cos(to_pos->lat) * cos(from_pos->lat) * cos(to_pos->lon - from_pos->lon)
        );
    return dist;
}

/**
 * \brief Horizontal move of point position
 */
int nmea_move_horz(
    const nmeaPOS *start_pos,   /**< Start position in radians */
    nmeaPOS *end_pos,           /**< Result position in radians */
    double azimuth,             /**< Azimuth (degree) [0, 359] */
    double distance             /**< Distance (km) */
    )
{
    nmeaPOS p1 = *start_pos;
    int RetVal = 1;

    distance /= NMEA_EARTHRADIUS_KM; /* Angular distance covered on earth's surface */
    azimuth = nmea_degree2radian(azimuth);

    end_pos->lat = asin(
        sin(p1.lat) * cos(distance) + cos(p1.lat) * sin(distance) * cos(azimuth));
    end_pos->lon = p1.lon + atan2(
        sin(azimuth) * sin(distance) * cos(p1.lat), cos(distance) - sin(p1.lat) * sin(end_pos->lat));

    if(NMEA_POSIX(isnan)(end_pos->lat) || NMEA_POSIX(isnan)(end_pos->lon))
    {
        end_pos->lat = 0; end_pos->lon = 0;
        RetVal = 0;
    }

    return RetVal;
}

#endif


