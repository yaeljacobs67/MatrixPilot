// This file is part of MatrixPilot.
//
//    http://code.google.com/p/gentlenav/
//
// Copyright 2009-2011 MatrixPilot Team
// See the AUTHORS.TXT file for a list of authors of MatrixPilot.
//
// MatrixPilot is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MatrixPilot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MatrixPilot.  If not, see <http://www.gnu.org/licenses/>.

////////////////////////////////////////////////////////////////////////////////
// Waypoint handling

// Move on to the next waypoint when getting within this distance of the current goal (in meters)
#define WAYPOINT_RADIUS 		10

// Origin Location
// When using relative waypoints, the default is to interpret those waypoints as relative to the
// plane's power-up location.  Here you can choose to use any specific, fixed 3D location as the
// origin point for your relative waypoints.
//
// USE_FIXED_ORIGIN should be 0 to use the power-up location as the origin for relative waypoints.
// Set it to 1 to use a fixed location as the origin, no matter where you power up.
// FIXED_ORIGIN_LOCATION is the location to use as the origin for relative waypoints.  It uses the
// format { X, Y, Z } where:
// X is Logitude in degrees * 10^7
// Y is Latitude in degrees * 10^7
// Z is altitude above sea level, in meters, as a floating point value.
// 
// If you are using waypoints for an autonomous landing, it is a good idea to set the altitude value
// to be the altitude of the landing point, and then express the heights of all of the waypoints with
// respect to the landing point.
// If you are using OpenLog, an easy way to determine the altitude of your landing point is to
// examine the telemetry after a flight, take a look in the .csv file, it will be easy to spot the
// altitude, expressed in meters.

// center of launch zone 40 4' 22.35"N 105 13' 48.88"W
//#define FIXED_ORIGIN_LOCATION	{ -1052302444, 400728750, 1587.7 }

// AAM East Field runway west X
//#define FIXED_ORIGIN_LOCATION	{ -1052136160, 398424410, 1812.0 }

// bench location
//#define FIXED_ORIGIN_LOCATION	{ -1050979310,  397501990, 1695.0 }


////////////////////////////////////////////////////////////////////////////////
// Waypoint definitions
// 
// Define the main course as:
// 
// const struct waypointDef waypoints[] = {
//						waypoint1 ,
//						waypoint2 ,
//						etc.
//						} ;
// 
// and the Failsafe RTL course as:
// 
// const struct waypointDef rtlWaypoints[] = {
//						waypoint1 ,
//						waypoint2 ,
//						etc.
//						} ;
// 
// A waypoint is defined as { { X , Y , Z } , F , CAM_VIEW }
// where X, Y, and Z are the three coordinates of the waypoint,
// F stores the flags/options for this waypoint,
// and CAM_VIEW is a 3D location to aim the camera as {CAM_X, CAM_Y, CAM_Z}, or just
// CAM_VIEW_LAUNCH to aim the camera at the launch point.
// (This must be provided even if no camera is used.)
// 
// Each waypoint can be defined as either a Relative waypoint or an Absolute waypoint.
//
// For a Relative waypoint,
// X, Y, and Z are distances in meters, relative to the initialization location of the board.
// X is positive as you move east
// Y is positive as you move north
// Z is the height
// 
// For an Absolute waypoint,
// X is Longitude in degrees * 10^+7
// Y is Latitude in degrees * 10^+7
// Z is altitude in meters relative to the initialization location of the board.
// As an example, the absolute waypoint { { -1219950467, 374124664, 100 }, F_ABSOLUTE } represents a point
// 100 meters above Baylands Park in Sunnyvale, CA, and will fly there normally (not inverted, etc.)
// ( Longitude = -121.9950467 degrees, Latitude = 37.4124664 degrees. )
// 
// Currently F can be set to: F_NORMAL, or any combination of:
// F_ABSOLUTE		- Waypoints are Relative by default, unless F_ABSOLUTE is specified.
// 
// F_TAKEOFF		- More quickly gain altitude at takeoff.
// F_INVERTED		- Navigate to this waypoint with the plane upside down. (only if STABILIZE_INVERTED_FLIGHT is set to 1 in options.h)
// F_HOVER			- Hover the plane until reaching this waypoint. (only if STABILIZE_HOVER is set to 1 in options.h)
//					  NOTE: while hovering, no navigation is performed, and throttle is under manual control.
// F_LOITER			- After reaching this waypoint, continue navigating towards this same waypoint.  Repeat until leaving waypoint mode.
// F_TRIGGER		- Trigger an action to happen when this waypoint leg starts.  (See the Trigger Action section of the options.h file.) 
// F_ALTITUDE_GOAL	- Climb or descend to the given altitude, then continue to the next waypoint.
// F_CROSS_TRACK	- Navigate using cross-tracking.  Best used for longer waypoint legs.
// F_LAND			- Navigate towards this waypoint with the throttle off.
// 
// 
// NOTE: Please be very careful when including inverted or hovering legs in a waypoints list.  Even if your plane does not fly well
//       in these orientations, or if you fly these legs without power, the UDB will keep on trying to maintain these orientations
//       which could lead to a crash.  If you try to manually recover from this behavior, remember to switch out of waypoiont mode
//       first, to avoid fighting the stabilization code.
// 
// 
// Camera View Points are now part of a waypoint definition file. The waypoint definition structure requires
// a camera viewpoint even if you do not have a camera on your plane. (until we move to having a flight
// planning language).  As a default, you can use the predefined CAM_VIEW_LAUNCH which points at { 0, 0, 0 }.
// 
// Camera Viewpoints are exactly like waypoint definitions. They define a point at which
// the camera will look in 3 dimensions. If you are using a waypoint relative to the initialisation of your 
// plane, then the camera viewpoint should also be relative e.g. "{ 32 , -22, 0 )".
// Camera waypoints can be absolute LAT and LONG, and camera target height is height above initalisation.
// This is the same as a fixed or absolute waypoint.
// Finally, do not mix relative waypoints and absolute camera viewpoint in the same line. A line should
// either use both a fixed waypoint and fixed camera viewpoint, or both relative.
// 
// 
// You do not need to specify how many points you have, the compiler will count them for you.
// You can use the facilities of the compiler to do some simple calculations in defining the course.
//
// To use waypoints, make sure FLIGHT_PLAN_TYPE is set to FP_WAYPOINTS in options.h.


////////////////////////////////////////////////////////////////////////////////
// waypoints[]
// 
// By default the only waypoint is defined to be 75 meters above the starting point.

//const struct waypointDef waypoints[] = {
//		{ {   0,   0, 75 } , F_NORMAL, CAM_VIEW_LAUNCH } ,  // return to, and loiter 75 meters above the startup position
//};

//const struct waypointDef waypoints[] = {
//    {{  100,  0, 50}, F_NORMAL, CAM_VIEW_LAUNCH}, //Waypoint 0
//    {{  100, 50, 50}, F_NORMAL, CAM_VIEW_LAUNCH}, //Waypoint 1
//    {{ -100, 50, 50}, F_NORMAL, CAM_VIEW_LAUNCH}, //Waypoint 2
//    {{ -100,  0, 50}, F_NORMAL, CAM_VIEW_LAUNCH}, //Waypoint 3
//};

#if 1
// AAM West Field runway center  39�50'31.97"N  105�13'10.17"W (105.2194917, 39.842213889)
#define USE_FIXED_ORIGIN		1
#define FIXED_ORIGIN_LOCATION	{ -1052194917, 398422138, 1817.0 }

#if 1
#pragma message "West Field lefthand pattern"

////////////////////////////////////////////////////////////////////////////////
// This is a lefthand pattern for takeoff to the east

#define USE_FIXED_ORIGIN		1

const struct waypointDef waypoints[] = {
	{ { 51, 3, 20 } , F_TAKEOFF , CAM_VIEW_LAUNCH } , //Waypoint 1
	{ { 98, 2, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 2
	{ { 109, 4, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 3
	{ { 111, 18, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 4
	{ { 110, 57, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 5
	{ { 104, 62, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 6
	{ { 94, 63, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 7
	{ { -100, 68, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 8
	{ { -109, 63, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 9
	{ { -115, 50, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 10
	{ { -114, 21, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 11
	{ { -109, 11, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 12
	{ { -99, 5, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 13
	{ { -84, 5, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 14
	{ { -67, 5, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 15
	{ { -51, 4, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 16
	{ { -28, 4, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 17
	{ { -10, 5, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 18
	{ { 9, 4, 10 } , F_CROSS_TRACK + F_TRIGGER , CAM_VIEW_LAUNCH } , //Waypoint 19
};
#else
#pragma message "West Field righthand pattern"

const struct waypointDef waypoints[] = {
	{ { -54, 4, 20 } , F_TAKEOFF , CAM_VIEW_LAUNCH } , //Waypoint 1
	{ { -91, 5, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 2
	{ { -100, 13, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 3
	{ { -102, 32, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 4
	{ { -103, 69, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 5
	{ { -95, 80, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 6
	{ { -84, 81, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 7
	{ { 79, 81, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 8
	{ { 91, 76, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 9
	{ { 97, 60, 35 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 10
	{ { 98, 22, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 11
	{ { 98, 5, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 12
	{ { 89, 1, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 13
	{ { 74, 1, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 14
	{ { 58, 1, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 15
	{ { 40, 2, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 16
	{ { 14, 3, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 17
	{ { -13, 3, 10 } , F_CROSS_TRACK + F_TRIGGER , CAM_VIEW_LAUNCH } , //Waypoint 18
};
#endif

#else
// AAM East Field runway center 39�50'31.83"N  105�12'44.81"W
#define USE_FIXED_ORIGIN 1
//#define FIXED_ORIGIN_LOCATION	{ -1052124472, 398421750, 1812.0 }
#if 1
#pragma message "low lefthand pattern waypoints"
// This is a lefthand pattern for takeoff to the east

//const struct waypointDef waypoints[] = {
//	{ { 30, -7, 5 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 1
//	{ { 100, -28, 5 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 2
//	{ { 101, 26, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 3
//	{ { -100, 88, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 4
//	{ { -102, 32, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 5
//	{ { -71, 22, 5 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 6
//	{ { -27, 9, 5 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 7
//	{ { 0, 0, 5 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 7
//	{ { 5, 0, 5 } , F_CROSS_TRACK + F_TRIGGER , CAM_VIEW_LAUNCH } , //Waypoint 7
//};
#else
#pragma message "righthand pattern waypoints"

// This is a lefthand pattern for takeoff to the east
// right hand pattern
const struct waypointDef waypoints[] = {
	{ { -27, 9, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 7
	{ { -71, 22, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 6
	{ { -102, 32, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 5
	{ { -100, 88, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 4
	{ { 101, 26, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 3
	{ { 100, -28, 20 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 2
	{ { 30, -7, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 1
	{ { 0, 0, 10 } , F_CROSS_TRACK + F_TRIGGER , CAM_VIEW_LAUNCH } , //Waypoint 7
	{ { -5, 0, 10 } , F_CROSS_TRACK , CAM_VIEW_LAUNCH } , //Waypoint 7
};
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// rtlWaypoints[]
// 
// This set of waypoints describes what to do when entering RTL mode.
// By default the only RTL waypoint is defined to be at the starting point, and to fly there unpowered.
// 
// Note that this set of waypoints is only used if FAILSAFE_TYPE is set to FAILSAFE_RTL in options.h.
// 
// WARNING: If you set this not to include the F_LAND flag, then be very careful during ground testing
// and after flights, since turning off the transmitter will cause the throttle to come on.

const struct waypointDef rtlWaypoints[] = {
		{ { 0, 30,  20 } , F_LAND, F_LOITER, CAM_VIEW_LAUNCH } 
} ;




////////////////////////////////////////////////////////////////////////////////
// Example waypoint lists
//
// Here are more examples of waypoints lists, commented out.  Note that these are
// all definitions of waypoints[], and if you want to use any of these as rtl waypoints,
// you'll have to change them to define rtlWaypoints[] instead of waypoints[].



////////////////////////////////////////////////////////////////////////////////
// This is an example course that makes a 100 meter square, 75 meters above the starting point, and
// then takes a photo and lands.
// 
// We first go to the south east corner of the square.
// Then on to the north east corner.
// The plane then uses ailerons to flip upside down, and heads towards the north west corner.
// Then we flip back over and head back to the south west corner.  
// We then take a photo, turn off the motor and head towards the middle of the square.
// When we fly past the middle, we turn back towards the middle, still without throttle, over and over until "landing".
// 
// Note that this is not likely to be an especially smooth landing...

/*
const struct waypointDef waypoints[] = {
		{ { 100,   0  , 75 } , F_NORMAL,   CAM_VIEW_LAUNCH } ,
		{ { 100, 100  , 75 } , F_NORMAL,   CAM_VIEW_LAUNCH } ,
		{ {   0, 100  , 75 } , F_INVERTED, CAM_VIEW_LAUNCH } ,
		{ {   0,   0  , 75 } , F_NORMAL,   CAM_VIEW_LAUNCH } ,
		{ {  50,  50  , 75 } , F_LOITER + F_TRIGGER + F_LAND, CAM_VIEW_LAUNCH } ,
} ;
*/


////////////////////////////////////////////////////////////////////////////////
// DIY Drones T3-2 Course
// 
// As a more complex example, here is the DIY Drones T3-2 contest course
// 
// Prior to flight, initialize the board at the center of the course.

/*
// CORNER is the absolute value of the X or Y coordinate at the corners of the course. 
#define CORNER 100

// CLEARANCE is an allowance for obstacles.
#define CLEARANCE 25

#define CAM_VIEW_2  { CORNER, CORNER, 0 } // Define a Camera ViewPoint to look at 100 ,100, 0

// Here is the T3 course definition:

const struct waypointDef waypoints[] = {
		{ {    CORNER  ,    CORNER  , CLEARANCE + 100 } , F_NORMAL, CAM_VIEW_LAUNCH } ,
		{ {    CORNER  ,  - CORNER  , CLEARANCE +  75 } , F_NORMAL, CAM_VIEW_LAUNCH } ,
		{ {  - CORNER  ,    CORNER  , CLEARANCE +  50 } , F_NORMAL, CAM_VIEW_LAUNCH } ,
		{ {  - CORNER  ,  - CORNER  , CLEARANCE +  25 } , F_NORMAL, CAM_VIEW_LAUNCH } ,
		{ {    CORNER  ,    CORNER  , CLEARANCE +  50 } , F_NORMAL, CAM_VIEW_LAUNCH } ,
		{ {    CORNER  ,  - CORNER  , CLEARANCE +  75 } , F_NORMAL, CAM_VIEW_LAUNCH } ,
		{ {  - CORNER  ,    CORNER  , CLEARANCE + 100 } , F_NORMAL, CAM_VIEW_LAUNCH } ,
		{ {  - CORNER  ,  - CORNER  , CLEARANCE +  75 } , F_NORMAL, CAM_VIEW_2 } ,
		{ {    CORNER  ,    CORNER  , CLEARANCE +  50 } , F_NORMAL, CAM_VIEW_2 } ,
		{ {    CORNER  ,  - CORNER  , CLEARANCE +  25 } , F_NORMAL, CAM_VIEW_2 } ,
		{ {  - CORNER  ,    CORNER  , CLEARANCE +  50 } , F_NORMAL, CAM_VIEW_2 } ,
		{ {  - CORNER  ,  - CORNER  , CLEARANCE +  75 } , F_NORMAL, CAM_VIEW_2 } ,
} ;
*/