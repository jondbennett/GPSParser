# GPSParser
An efficient, portable GPS parser (works on Arduino)
Inspired by: TinyGPS (Mikal Hart) https://github.com/mikalhart/TinyGPS
and the NMEA library http://nmea.sourceforge.net

This is a solid, simple-to-use GPS parser that will work on almost any platform. I've tested it on Windoze, Linux, and Arduino.
It has no concept of a serial port. You get GPS data from wherever you get it then pass it to the parse method of the CGPSParser object.
After passing it some data you can request the parser's GPSParserData object. All of its member variables are public so just
access whatever you want, but don't be stupid and change any of the member variables.
You can have as many instances as you wish.

The GPSDefs.h file lets you indicate (at compile time) which GPS sentences you want to parse. Any data in the GPSParserData object
that is unnecessary is removed from the structure at compile time so the memory footprint is as small as possible.

So...
Instantiate the object:
CGPSParser gpsParser;

Reset it whenever you want...
gpsParser.reset();

Give it some data to parse
unsigned char dataBuffer[???];

gpsParser.parse(dataBuffer, dataLen);

And get its data
double latitude;
if(gpsParser.getGPSData().m_GPSLocked)
	latitude = gpsParser.getGPSData().m_position.m_lat;

Finally, there is a CGPSPos object that can be used to measure distance between two points, heading from one point to another,
and deal with movement. It can be ordered to displace itself by some distance along a radial.


