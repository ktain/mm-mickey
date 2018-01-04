#include "main.h"

void loadSettings(void) {

	/* Default settings */
	
	// Location
	curPosX = 0;	 // 0 is bottom left, horizontal
	curPosY = 0;   // 0 is bottom left, vertical
	orientation = NORTH;
	
	// Search settings
	useBacktracking = 1;
	useAlignment = 1;
	useFrontWallCalibration = 1;
	updateAdjacentCells = 1;
	
	// Speed settings
	maxPwm = 999;				// max motor pwm, 12.4v supply
	stopSpeed = 0;			// m/s
	searchSpeed = 0.5;	// m/s
	turnSpeed = 0.5;		// m/s
	traceSpeed = 0.9;		// Backtracking
	straightSpeed = 1.0;	// Speed run
	calibrateSpeed = 0.2;	// IR Sensor calibration
	calibrateAccX = 0.1;
	calibrateDecX = 0.1;
	straightAccX = 2.0;				// m/s/s
	straightDecX = 2.0;				// m/s/s
	straightAccW = 1000;		// deg/s/s, used for straights
	straightDecW = 1000;  	// deg/s/s, used for straights
	straightKpX = 2*20;	// used for straights
	straightKdX = 3*20;
	straightKpW = 0.6*20;
	straightKdW = 20*20;
	speedRunProfile = 0;
	searchProfile = 0;
	pivotTurnDelay = 100;
	

	// Load IR sensor settings from flash
	loadSensorData();
	
	// IR Sensor constants
	LDMidVal = LDSensorLookup;
	RDMidVal = RDSensorLookup;
	leftWallThreshold = LDMidVal/4;			// when mouse is at half a cell distance
	rightWallThreshold = RDMidVal/4;
	frontWallThresholdL = LFSensorLookup[200];
	frontWallThresholdR = RFSensorLookup[200];
	approachThreshold = sensorLookup[100];		// Threshold for angular correction
	signalThreshold = sensorLookup[70];		// Detect finger
	sensorScale = 200*20;				// lower == more feedback
	
	
	// Alignment
	alignErrorTolerance = 2;// mm
	alignSpeed = 0.1;	// m/s
	alignTime = 50;	// for angular correction
	alignSensorScale = 100*20;	// lower == more feedback
	alignAccX = 0.6;	// m/s/s
	alignDecX = 0.6;	// m/s/s
	alignAccW = 100;	// deg/s/s
	alignDecW = 100;	// deg/s/s
	alignKpW = 0.3*20;
	alignKdW = 40*20;
	hugPwm = 200;
	hugTime = 300;		// ms
	hugSensorScale = 0.5;	// higher == more feedback
	
	// Measurements
	cellDistance = 962;		// 969 counts
	wheelDistance = 71.0; 		// mm, if overturn -> reduce
	centerToBackDist = 45;	// mm
	frontEdgeDist = 29;			// 84-(mouseLength-centerToBackDist)
	frontEdgeDistTurn = 35;
	
	// Print settings
	showTrace = 1;

}
