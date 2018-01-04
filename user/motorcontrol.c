#include "main.h"

// System controlled
bool useMotorControl = 0;
bool useIRSensors = 0;

// User controlled
bool useAlignment;
bool speedRunProfile;
bool searchProfile;
bool useFrontWallCalibration;

float leftEncCount = 0;
float rightEncCount = 0;
float encCount = 0;
float oldLeftEncCount = 0;
float oldRightEncCount = 0;
float leftEncChange = 0;
float rightEncChange = 0;
float encChange = 0;			// current speed in counts/ms
float distanceLeft = 0;

float targetSpeedX = 0; // speed in counts/ms
float targetSpeedW = 0;	// speed in counts/ms

float curSpeedX = 0;		
float curSpeedW = 0;
float encFeedbackX = 0;
float encFeedbackW = 0;
float gyroFeedback = 0;
float errorX = 0;
float errorW = 0;
float oldErrorX = 0;
float oldErrorW = 0;
float posPwmX = 0;
float posPwmW = 0;
float leftPwmTotal;
float rightPwmTotal;

// Speed settings
float maxPwm;
float stopSpeed;
float searchSpeed;
float turnSpeed;
float traceSpeed;
float straightSpeed;
float alignSpeed;
float calibrateSpeed;
float calibrateAccX;
float calibrateDecX;

// Constant variables
float cellDistance;		// counts
float wheelDistance;  // mm
float sensorScale;
float centerToBackDist;
float frontEdgeDist;  
float frontEdgeDistTurn;  
float alignSensorScale;

float accX;	// m/s/s
float decX;
float accW;	// deg/s/s
float decW;
float alignAccX;
float alignDecX;
float alignAccW;
float alignDecW;
float alignKdW;
float alignKpW;
float straightAccX;
float straightDecX;
float straightAccW;
float straightDecW;
float straightKdX;
float straightKpX;
float straightKpW;
float straightKdW;

// Motor encoder PID
float kpX;
float kdX;
float kpW;
float kdW;

int32_t hugTime;
int32_t hugPwm;
float hugSensorScale;

void setLeftPwm(int32_t pwm) {
	
	if (pwm > maxPwm)
		pwm = maxPwm;
	else if (pwm < -maxPwm)
		pwm = -maxPwm;
	
	if (USESLOWDECAY) {
		// Slow decay
		if (pwm >= 0) {
				LPWMA = maxPwm;
				LPWMB	= maxPwm - pwm; 
		}          
		else	{        
				pwm = -pwm;
				LPWMA = maxPwm - pwm;
				LPWMB	= maxPwm; 
		}
	}
	else {
		// Fast decay
		if(pwm >= 0)	{
				LPWMA = pwm;
				LPWMB	= 0; 
		}          
		else	{   
				pwm = -pwm;
				LPWMA = 0;
				LPWMB	= pwm; 
		}
	}
}

void setRightPwm(int32_t pwm) {
	if (pwm > maxPwm)
		pwm = maxPwm;
	else if (pwm < -maxPwm)
		pwm = -maxPwm;	

	if (USESLOWDECAY) {
		// Slow decay
		if (pwm >= 0) {
			RPWMA = maxPwm;
			RPWMB = maxPwm - pwm;
		}
		else {
			pwm = -pwm;
			RPWMA = maxPwm - pwm;
			RPWMB = maxPwm;
		}
	}
	else {
		// Fast decay
		if(pwm >= 0)	{
				RPWMA = pwm;
				RPWMB	= 0; 
		}          
		else	{    
				pwm = -pwm;
				RPWMA = 0;
				RPWMB	= pwm; 
		}
	}
}


// Update motor speed to match target speed
void updateSpeed(void) {
	
	// Get data from encoders and update distance left to travel
	leftEncCount = getLeftEncCount();
	rightEncCount = getRightEncCount();
	
	leftEncChange = leftEncCount - oldLeftEncCount;
	rightEncChange = rightEncCount - oldRightEncCount;
	encChange = (leftEncChange + rightEncChange)/2;
	
	oldLeftEncCount = leftEncCount;
	oldRightEncCount = rightEncCount;
	
	leftEncCount += leftEncChange;
	rightEncCount += rightEncChange;
	encCount = (leftEncCount + rightEncCount)/2;
	
	// Update current speed
	if (curSpeedX < targetSpeedX) {
		curSpeedX += mm_to_counts(accX)/1000;
		if (curSpeedX > targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	else if (curSpeedX > targetSpeedX) {
		curSpeedX -= mm_to_counts(decX)/1000;
		if (curSpeedX < targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	
	if (curSpeedW < targetSpeedW) {
		curSpeedW += deg_to_counts(accW)/1000/1000;
		if (curSpeedW > targetSpeedW)
			curSpeedW = targetSpeedW;
	}
	else if (curSpeedW > targetSpeedW) {
		curSpeedW -= deg_to_counts(decW)/1000/1000;
		if (curSpeedX < targetSpeedW)
			curSpeedW = targetSpeedW;
	}
	
	distanceLeft -= encChange;
	
	// PD control 
	float rotationalFeedback;
	
	encFeedbackX = (rightEncChange + leftEncChange)/2;
	encFeedbackW = (rightEncChange - leftEncChange)/2;
	
	rotationalFeedback = encFeedbackW;
	
	
	// IR Sensor feedback
	if (useIRSensors)
		rotationalFeedback -= getSensorError()/sensorScale;
	
	
	errorX += curSpeedX - encFeedbackX;
	errorW += curSpeedW - rotationalFeedback;
	
	posPwmX = (kpX * errorX + kdX * (errorX - oldErrorX));
	posPwmW = (kpW * errorW + kdW * (errorW - oldErrorW));
	
	oldErrorX = errorX;
	oldErrorW = errorW;
	
	leftPwmTotal = posPwmX - posPwmW;
	rightPwmTotal = posPwmX + posPwmW;
	
	// Decrease right PWM if left saturates
	if (leftPwmTotal > maxPwm)
		rightPwmTotal -= leftPwmTotal - maxPwm;
	else if (leftPwmTotal < -maxPwm)
		rightPwmTotal += maxPwm - leftPwmTotal;
	
	// Decrease left PWM if right saturates
	if (rightPwmTotal > maxPwm)
		leftPwmTotal -= rightPwmTotal - maxPwm;
	else if (rightPwmTotal < -maxPwm) 
		leftPwmTotal += maxPwm - rightPwmTotal;
	
	setLeftPwm(leftPwmTotal);
	setRightPwm(rightPwmTotal);
}


// Get deceleration needed given distance left to travel, current speed, and final speed
float getDecNeeded(int32_t d, float Vi, float Vf) {
	float decNeeded;
	if (d == 0) {
		d = 1;
	}
	decNeeded = (Vf*Vf - Vi*Vi)/d/2;
	return (decNeeded < 0)? -decNeeded: decNeeded;	// Always returns non-negative
}

// convert counts to mm
float counts_to_mm(float counts) {
	return (counts/(cellDistance/180));
}


// convert mm to counts
float mm_to_counts(float mm) {
	return (mm*(cellDistance/180));
}

// convert counts to deg
float counts_to_deg(float counts) {
	return (counts/(3.14159f*wheelDistance*cellDistance/180/360));
}

// convert deg to counts
float deg_to_counts(float deg) {
	return (deg*(3.14159f*wheelDistance*cellDistance/180/360));
}

void enableMotorControl(void) {
	resetMotorParameters();
	useMotorControl = 1;
}

void disableMotorControl(void) {
	useMotorControl = 0;
	resetMotorParameters();
}

void resetMotorParameters(void) {
	
	leftEncCount = 0;
	rightEncCount = 0;
	encCount = 0;
	oldLeftEncCount = 0;
	oldRightEncCount = 0;
	leftEncChange = 0;
	rightEncChange = 0;
	encChange = 0;
	distanceLeft = 0;

	targetSpeedX = 0;
	targetSpeedW = 0;

	curSpeedX = 0;
	curSpeedW = 0;
	encFeedbackX = 0;
	encFeedbackW = 0;
	gyroFeedback = 0;
	errorX = 0;
	errorW = 0;
	oldErrorX = 0;
	oldErrorW = 0;
	posPwmX = 0;
	posPwmW = 0;
	
	setLeftPwm(0);
	setRightPwm(0);
	
	resetLeftEncCount();
	resetRightEncCount();
}


/**
 *	Straight movement
 */
void moveStraight(float cells, float maxSpeed, float endSpeed) {
	accX = straightAccX;
	decX = straightDecX;
	accW = straightAccW;
	decW = straightDecW;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;
	
	distanceLeft = cells*cellDistance;
	// Forward movement
	if (cells > 0) {
		useIRSensors = 1;
		targetSpeedX = mm_to_counts(maxSpeed);
		while(distanceLeft > 0) {
			if (getDecNeeded(distanceLeft, curSpeedX, mm_to_counts(endSpeed)) > mm_to_counts(decX)/1000) {
				targetSpeedX = mm_to_counts(endSpeed);
			}
			else {
				targetSpeedX = mm_to_counts(maxSpeed);
			}
		}
		targetSpeedX = mm_to_counts(endSpeed);
	}
	
	// Backward movement
	else if (cells < 0) {
		// swap acc/dec when reverse
		accX = straightDecX;
		decX = straightAccX;
		targetSpeedX = -mm_to_counts(maxSpeed);
		while(distanceLeft < 0) {
			if (getDecNeeded(distanceLeft, curSpeedX, mm_to_counts(endSpeed)) > mm_to_counts(accX)/1000) {
				targetSpeedX = -mm_to_counts(endSpeed);
			}
			else {
				targetSpeedX = mm_to_counts(maxSpeed);
			}
		}
		targetSpeedX = mm_to_counts(endSpeed);
	}
	targetSpeedX = mm_to_counts(endSpeed);
	distanceLeft = 0;
	useIRSensors = 0;
	ALL_LED_OFF;
}


/**
 *	Angular movement
 *  +speedW turns right, -speedW turns left
 *	time is in ms, speedX is in m/s, speedW is in deg/s, acc is in deg/s/s
 */
void turn(struct turnProfile profile) {
	
	// if has front wall, compensate distance before turn
	if (((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
	     (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY))) &&
	      useFrontWallCalibration && profile.d1 > 0) {
		calibrateTurn(profile.speedX, profile.endSpeed);
		if (profile.d1 - 10 > 0)
			moveStraight(mm_to_counts(profile.d1-10)/cellDistance, profile.endSpeed, profile.endSpeed);
	}
	// Otherwise, no front wall compensation
	else if (profile.d1 > 0){
		moveStraight(mm_to_counts(profile.d1)/cellDistance, profile.endSpeed, profile.endSpeed);
	}
	
	if (profile.speedW > -1 && profile.speedW < 1)
		delay_ms(pivotTurnDelay);
	
	accW = profile.accW;
	decW = profile.decW;
	
	int32_t curt = millis();
	
	// T1
	kpW = profile.kpW13;
	kdW = profile.kdW13;
	targetSpeedW = deg_to_counts(-profile.speedW)/1000;
	elapse_ms(profile.t1, curt);
	// T2
	kpW = profile.kpW2;
	kdW = profile.kdW2;
	elapse_ms(profile.t1 + profile.t2, curt);
	// T3
	kpW = profile.kpW13;
	kdW = profile.kdW13;
	targetSpeedW = stopSpeed;
	elapse_ms(profile.t1 + profile.t2 + profile.t3, curt);

	moveStraight(mm_to_counts(profile.d2)/cellDistance, profile.speedX, profile.endSpeed);
	
	if (profile.speedW > -1 && profile.speedW < 1)
		delay_ms(pivotTurnDelay);
}


/**
 * Calibrate before turn
 */
void calibrateTurn(float maxSpeed, float endSpeed) {
	accX = straightAccX;
	decX = straightDecX;
	accW = straightAccW;
	decW = straightDecW;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;
	
	useIRSensors = 1;
	
	targetSpeedX = mm_to_counts(endSpeed);

	// If overshoot, compensate distance up to 10mm
	while((LFSensor < LFSensorLookup[80 + (int32_t)frontEdgeDistTurn]) && (RFSensor < RFSensorLookup[80 + (int32_t)frontEdgeDistTurn]));
	
	targetSpeedX = mm_to_counts(endSpeed);

	useIRSensors = 0;
}


void align(void){
	int32_t tempSensorScale = sensorScale;
	accX = alignAccX;
	decX = alignDecX;
	accW = alignAccW;
	decW = alignDecW;
	kpX = straightKpX;
	kdX = straightKdX;
	kdW = alignKdW;
	kpW = alignKpW;
	sensorScale = alignSensorScale;
	int32_t distanceLeft;
	useIRSensors = 1;
	
	// Perform alignment
	for (int32_t i = 0; i < 1; i++) {
		// Too close, move back
		if (sensorLookup[(int32_t)frontEdgeDist - alignErrorTolerance] < (LFSensor + RFSensor)/2) {
			// swap acc/dec when reverse
			accX = alignDecX;
			decX = alignAccX;
			
			distanceLeft = 0;
			targetSpeedX = -mm_to_counts(alignSpeed);
			while(sensorLookup[(int32_t)frontEdgeDist] < (LFSensor + RFSensor)/2) {
				// get dist from front wall
				int32_t i;
				for(i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
					if (sensorLookup[i] <= (LFSensor + RFSensor)/2) {
						break;
					}
				}
				
				// distance left should be negative
				distanceLeft = mm_to_counts(i) - mm_to_counts(frontEdgeDist);
				if (distanceLeft > 0)
					distanceLeft = 0;
				
				// if sensor reading is greater than all stored values, move backward
				if (i == SENSOR_DATA_ARRAY_SIZE)
					targetSpeedX = -mm_to_counts(alignSpeed);
				else if (getDecNeeded(distanceLeft, curSpeedX, stopSpeed) < mm_to_counts(alignAccX)/1000) { // swap acc/dec when reverse
					targetSpeedX = -mm_to_counts(alignSpeed);
				}
				else {
					targetSpeedX = stopSpeed;
				}
			}
		}
		
		// Too far, move forward
		else if (sensorLookup[(int32_t)frontEdgeDist + alignErrorTolerance] > (LFSensor + RFSensor)/2) {
			distanceLeft = 0;
			targetSpeedX = mm_to_counts(alignSpeed);
			while(sensorLookup[(int32_t)frontEdgeDist] > (LFSensor + RFSensor)/2) {
				int32_t i;
				for(i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
					if (sensorLookup[i] <= (LFSensor + RFSensor)/2) {
						break;
					}
				}
				distanceLeft = mm_to_counts(i - (int32_t)frontEdgeDist);
				if (distanceLeft < 0)
					distanceLeft = 0;
				
				// if sensor reading is greater than all stored values
				if (i == SENSOR_DATA_ARRAY_SIZE)
					targetSpeedX = mm_to_counts(alignSpeed);
				else if (getDecNeeded(distanceLeft, curSpeedX, stopSpeed) < mm_to_counts(alignDecX)/1000) {
					targetSpeedX = mm_to_counts(alignSpeed);
				}
				else {
					targetSpeedX = stopSpeed;
				}
			}
		}
	}
	targetSpeedX = stopSpeed;
	delay_ms(alignTime);	// prevent overshoot
	sensorScale = tempSensorScale;
	useIRSensors = 0;
}


/* Hug Front Wall */
void hugFrontWall(int32_t LSensorVal, int32_t RSensorVal) {
	resetMotorParameters();
	uint32_t useMotorControlState = useMotorControl;
	useMotorControl = 0;
	int32_t tempPwm = maxPwm;
	int curt = millis();
	maxPwm = hugPwm;
	while(millis() - curt < hugTime) {
		setLeftPwm(hugSensorScale*(LSensorVal - LFSensor));
		setRightPwm(hugSensorScale*(RSensorVal - RFSensor));
	}
	maxPwm = tempPwm;
	resetMotorParameters();
	useMotorControl = useMotorControlState;
}
