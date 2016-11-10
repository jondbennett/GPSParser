/////////////////////////////////////////////////////////////////////////
// Author: Jon Bennett (jon@jondbennett.com) http://jondbennett.com
//
// GPS Navigation calculation object
// NOTE: All distances and displacements are in meters. Uses
// (that it *requires* the NMEA parser library
/////////////////////////////////////////////////////////////////////////
#ifndef CGPSPOS_H
#define CGPSPOS_H

/////////////////////////////////////////////////////////////////////////
// class CGPSPos
/////////////////////////////////////////////////////////////////////////
class CGPSPos
{
protected:
	double m_lat, m_lon;

public:

	// Structors
	CGPSPos();
	CGPSPos(double _dLat, double _dLon);
	CGPSPos(const CGPSPos &_pos);
	CGPSPos &operator=(const CGPSPos &_pos);
	bool operator==(const CGPSPos &_pos);

	// Accessors
	double getLat() const { return m_lat; }
	double getLon() const { return m_lon; }

	void setLat(double _dLat) { m_lat = _dLat; }
	void setLon(double _dLon) { m_lon = _dLon; }

	void setPosition(double _dLat, double _dLon);
	void getPosition(double *_pdLat, double *_pdLon);
	void clear();

	// Calculations
	double distanceToInMeters(CGPSPos _pos);
	double trueHeadingTo(CGPSPos _pos);

	bool displaceByMeters(double _dRadial, double _dDistance);
};


#endif

