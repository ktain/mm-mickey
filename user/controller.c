#include "main.h"

// d1 t1 t2 t3 d2 startSpeed speedX endSpeed speedW accW decW kpW13 kdW13 kpW2 kdW2

// Pivot turns
struct turnProfile pivotLeft90Profile[3] = {	
	0, 15, 285, 15, 0, 0, 0, 0, -300, 20000, 20000, 30, 50, 20, 100,
	0, 15, 285, 15, 0, 0, 0, 0, -300, 20000, 20000, 30, 50, 20, 100,
};

struct turnProfile pivotRight90Profile[3] = {		
	0, 15, 285, 15, 0, 0, 0, 0, 300, 20000, 20000, 30, 50, 20, 100,
	0, 15, 285, 15, 0, 0, 0, 0, 300, 20000, 20000, 30, 50, 20, 100,
};

struct turnProfile pivotLeft180Profile[3] = {
	0, 15, 585, 15, 0, 0, 0, 0, -300, 20000, 20000, 30, 50, 20, 100,
	0, 15, 585, 15, 0, 0, 0, 0, -300, 20000, 20000, 30, 50, 20, 100,
};

struct turnProfile pivotRight180Profile[3] = {
	0, 15, 585, 15, 0, 0, 0, 0, 300, 20000, 20000, 30, 50, 20, 100,
	0, 15, 585, 15, 0, 0, 0, 0, 300, 20000, 20000, 30, 50, 20, 100,
};

// Search turns
struct turnProfile curveLeft90Profile[3] = {
	20, 22, 183, 22, 20, 0.5, 0.5, 0.5, -440, 20000, 20000, 30, 50, 20, 100,
	20, 28, 134, 28, 20, 0.6, 0.6, 0.6, -555, 20000, 20000, 30, 50, 20, 100,
};

struct turnProfile curveRight90Profile[3] = {
	20, 22, 183, 22, 20, 0.5, 0.5, 0.5, 440, 20000, 20000, 30, 50, 20, 100,
	20, 28, 134, 28, 20, 0.6, 0.6, 0.6, 555, 20000, 20000, 30, 50, 20, 100,
};

// Diagonal run turns
struct turnProfile curveLeft90SSProfile[3] = {
	4, 36, 128, 36, 41, 1.3, 1.3, 1.3, -545, 15000, 15000, 0.7, 20, 0.7, 25
};

struct turnProfile curveRight90SSProfile[3] = {
	10, 36, 128, 36, 45, 1.3, 1.3, 1.3, 545, 15000, 15000, 0.7, 20, 0.7, 25
};

struct turnProfile curveLeft90DDProfile[3] = {
	0, 82, 138, 82, 0, 0.7, 0.7, 0.7, -410, 5000, 5000, 0.7, 20, 0.7, 25
};

struct turnProfile curveRight90DDProfile[3] = {
	0, 82, 138, 82, 0, 0.7, 0.7, 0.7, 410, 5000, 5000, 0.7, 20, 0.7, 25
};

struct turnProfile curveLeft45SDProfile[3] = {
	2, 75, 75, 75, 33, 0.7, 0.7, 0.7, -300, 4000, 4000, 0.7, 20, 0.7, 25,	
};

struct turnProfile curveRight45SDProfile[3] = {
	10, 75, 75, 75, 27, 0.7, 0.7, 0.7, 300, 4000, 4000, 0.7, 20, 0.7, 25,		
};

struct turnProfile curveLeft45DSProfile[3] = {
	105, 30, 45, 30, 40, 0.7, 0.7, 0.7, -590, 20000, 20000, 0.7, 20, 0.7, 25,	
};

struct turnProfile curveRight45DSProfile[3] = {
	110, 30, 45, 30, 35, 0.7, 0.7, 0.7, 590, 20000, 20000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveLeft135SDProfile[3] = {
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, -580, 15000, 15000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight135SDProfile[3] = {
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, 580, 15000, 15000, 0.7, 20, 0.7, 25,
};


struct turnProfile curveLeft135DSProfile[3] = {
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, -580, 15000, 15000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight135DSProfile[3] = {
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, 580, 15000, 15000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveLeft180Profile[3] = {
	40, 42, 248, 52, 44, 1.0, 1.0, 1.0, -620, 15000, 15000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight180Profile[3] = {
	40, 42, 251, 52, 40, 1.0, 1.0, 1.0, 620, 15000, 15000, 0.7, 20, 0.7, 25,
};



void curveLeft90(void) {
	turn(curveLeft90Profile[searchProfile]);
}

void curveRight90(void) {
	turn(curveRight90Profile[searchProfile]);
}

void curveLeft90SS(void) {
	turn(curveLeft90SSProfile[speedRunProfile]);
}

void curveRight90SS(void) {
	turn(curveRight90SSProfile[speedRunProfile]);
}

void curveLeft90DD(void) {
	turn(curveLeft90DDProfile[speedRunProfile]);
}

void curveRight90DD(void) {
	turn(curveRight90DDProfile[speedRunProfile]);
}

void curveLeft45SD(void) {
	turn(curveLeft45SDProfile[speedRunProfile]);
}

void curveRight45SD(void) {
	turn(curveRight45SDProfile[speedRunProfile]);
}

void curveLeft45DS(void) {
	turn(curveLeft45DSProfile[speedRunProfile]);
}

void curveRight45DS(void) {
	turn(curveRight45DSProfile[speedRunProfile]);
}

void curveLeft135SD(void) {
	turn(curveLeft135SDProfile[speedRunProfile]);
}

void curveRight135SD(void) {
	turn(curveRight135SDProfile[speedRunProfile]);
}

void curveLeft135DS(void) {
	turn(curveLeft135DSProfile[speedRunProfile]);
}

void curveRight135DS(void) {
	turn(curveRight135DSProfile[speedRunProfile]);
}

void curveLeft180SS(void) {
	turn(curveLeft180Profile[speedRunProfile]);
}

void curveRight180SS(void) {
	turn(curveRight180Profile[speedRunProfile]);
}

void pivotLeft90(void) {
	turn(pivotLeft90Profile[searchProfile]);
}

void pivotRight90(void) {
	turn(pivotRight90Profile[searchProfile]);
}

void pivotLeft180(void) {
	turn(pivotLeft180Profile[searchProfile]);
}

void pivotRight180(void) {
	turn(pivotRight180Profile[searchProfile]);
}

void moveN(void) {
	if (orientation == NORTH)
		moveStraight(1, searchSpeed, searchSpeed);
	else if (orientation == EAST)
		curveLeft90();
	else if (orientation == SOUTH)
		moveBack();
	else if (orientation == WEST)
		curveRight90();
	
	curPosY++;
	orientation = NORTH;
}

void moveE(void) {
	if (orientation == EAST)
		moveStraight(1, searchSpeed, searchSpeed);
	else if (orientation == SOUTH)
		curveLeft90();
	else if (orientation == WEST)
		moveBack();
	else if (orientation == NORTH)
		curveRight90();
	
	curPosX++;
	orientation = EAST;
}

void moveS(void) {
	if (orientation == SOUTH)
		moveStraight(1, searchSpeed, searchSpeed);
	else if (orientation == WEST)
		curveLeft90();
	else if (orientation == NORTH)
		moveBack();
	else if (orientation == EAST)
		curveRight90();
	
	curPosY--;
	orientation = SOUTH;
}

void moveW(void) {	
	if (orientation == WEST)
		moveStraight(1, searchSpeed, searchSpeed);
	else if (orientation == NORTH)
		curveLeft90();
	else if (orientation == EAST)
		moveBack();
	else if (orientation == SOUTH)
		curveRight90();
	
	curPosX--;
	orientation = WEST;
}

void moveBack(void) {

	bool hasFrontWall = (LFSensor > frontWallThresholdL && RFSensor > frontWallThresholdR)? 1: 0;
	bool hasLeftWall = (LDSensor > leftWallThreshold)? 1: 0;
	bool hasRightWall = (RDSensor > rightWallThreshold)? 1: 0;

	int32_t LDOffset = LDMidVal - LDSensor;	// Higher = further away
	int32_t RDOffset = RDMidVal - RDSensor;
	
	// Decelerate half a cell distance
	if (hasFrontWall && useAlignment) {
		moveStraight(0.4, turnSpeed, alignSpeed);
		align();
	}
	else {
		moveStraight(0.5, turnSpeed, stopSpeed);
		delay_ms(50);
	}
	
	// Turn back
	if (hasLeftWall && (LDOffset > RDOffset) && useAlignment) {
		pivotLeft90();
		align();
		pivotLeft90();
	}
	else if (hasRightWall && (RDOffset > LDOffset) && useAlignment) {
		pivotRight90();
		align();
		pivotRight90();
	}
	else if (hasLeftWall && useAlignment) {
		pivotLeft90();
		align();
		pivotLeft90();
	}
	else if (hasRightWall && useAlignment) {
		pivotRight90();
		align();
		pivotRight90();
	}
	else {
		// Randomly pivot 180
		(millis()%2)? pivotLeft180():pivotRight180();
	}
	
	// Move forward half a cell distance
	moveStraight(0.5, turnSpeed, turnSpeed);
}


// Pivot the mouse north
void pivotN(void) {
	if (orientation == EAST)
		pivotLeft90();
	else if (orientation == SOUTH) {
		// If has left wall, align with left wall
		if (useAlignment && hasEastWall(curPosX, curPosY)) {
			pivotLeft90();
			align();
			pivotLeft90();
		}
		// Else if has right wall, align with right wall
		else if (useAlignment && hasWestWall(curPosX, curPosY)) {
			pivotRight90();
			align();
			pivotRight90();
		}
		else {
			// Randomly pivot 180
			(millis()%2)? pivotLeft180():pivotRight180();
		}
	}
	else if (orientation == WEST)
		pivotRight90();
	
	orientation = NORTH;
}


// Pivot the mouse east
void pivotE(void) {
	if (orientation == NORTH)
		pivotRight90();
	else if (orientation == SOUTH)
		pivotLeft90();
	else if (orientation == WEST) {
		// If has left wall, align with left wall
		if (useAlignment && hasSouthWall(curPosX, curPosY)) {
			pivotLeft90();
			align();
			pivotLeft90();
		}
		// Else if has right wall, align with right wall
		else if (useAlignment && hasNorthWall(curPosX, curPosY)) {
			pivotRight90();
			align();
			pivotRight90();
		}
		else {
			// Randomly pivot 180
			(millis()%2)? pivotLeft180():pivotRight180();
		}
	}

	orientation = EAST;
}


// Pivot the mouse south
void pivotS(void) {
	if (orientation == NORTH) {
		// If has left wall, align with left wall
		if (useAlignment && hasWestWall(curPosX, curPosY)) {
			pivotLeft90();
			align();
			pivotLeft90();
		}
		// Else if has right wall, align with right wall
		else if (useAlignment && hasEastWall(curPosX, curPosY)) {
			pivotRight90();
			align();
			pivotRight90();
		}
		else {
			// Randomly pivot 180
			(millis()%2)? pivotLeft180():pivotRight180();
		}
	}
	else if (orientation == EAST)
		pivotRight90();
	else if (orientation == WEST)
		pivotLeft90();

	orientation = SOUTH;
}


// Pivot the mouse west
void pivotW(void) {
	if (orientation == NORTH)
		pivotLeft90();
	else if (orientation == EAST) {
		// If has left wall, align with left wall
		if (useAlignment && hasNorthWall(curPosX, curPosY)) {
			pivotLeft90();
			align();
			pivotLeft90();
		}
		// Else if has right wall, align with right wall
		else if (useAlignment && hasSouthWall(curPosX, curPosY)) {
			pivotRight90();
			align();
			pivotRight90();
		}
		else {
			// Randomly pivot 180
			(millis()%2)? pivotLeft180():pivotRight180();
		}
	}
	else if (orientation == SOUTH)
		pivotRight90();

	orientation = WEST;
}

// Check if approaching front wall
bool approachingWall (void) {
	if ( (LFSensor > approachThreshold) && (RFSensor > approachThreshold) )
		return 1;
	else
		return 0;
}

float getStartSpeed(int32_t profile) {
	switch(profile) {
		case SS0: 	return straightSpeed;
		case SS90L: 	return curveLeft90SSProfile[speedRunProfile].startSpeed;
		case SS90R: 	return curveRight90SSProfile[speedRunProfile].startSpeed;
		case SS180L: 	return curveLeft180Profile[speedRunProfile].startSpeed;
		case SS180R: 	return curveRight180Profile[speedRunProfile].startSpeed;
		case SD45L: 	return curveLeft45SDProfile[speedRunProfile].startSpeed;
		case SD45R: 	return curveRight45SDProfile[speedRunProfile].startSpeed;
		case SD135L:  return curveLeft135SDProfile[speedRunProfile].startSpeed;
		case SD135R:  return curveRight135SDProfile[speedRunProfile].startSpeed;
		case DS45L:   return curveLeft45DSProfile[speedRunProfile].startSpeed;
		case DS45R:   return curveRight45DSProfile[speedRunProfile].startSpeed;
		case DS135L:  return curveLeft135DSProfile[speedRunProfile].startSpeed;
		case DS135R:  return curveRight135DSProfile[speedRunProfile].startSpeed;
		case DD90L:   return curveLeft90DDProfile[speedRunProfile].startSpeed;
		case DD90R:   return curveRight90DDProfile[speedRunProfile].startSpeed;
		default:			return 0;
	}
}

float getEndSpeed(int32_t profile) {
		switch(profile) {
		case SS0: 	return straightSpeed;
		case SS90L: 	return curveLeft90SSProfile[speedRunProfile].endSpeed;
		case SS90R: 	return curveRight90SSProfile[speedRunProfile].endSpeed;
		case SS180L: 	return curveLeft180Profile[speedRunProfile].endSpeed;
		case SS180R: 	return curveRight180Profile[speedRunProfile].endSpeed;
		case SD45L: 	return curveLeft45SDProfile[speedRunProfile].endSpeed;
		case SD45R: 	return curveRight45SDProfile[speedRunProfile].endSpeed;
		case SD135L:  return curveLeft135SDProfile[speedRunProfile].endSpeed;
		case SD135R:  return curveRight135SDProfile[speedRunProfile].endSpeed;
		case DS45L:   return curveLeft45DSProfile[speedRunProfile].endSpeed;
		case DS45R:   return curveRight45DSProfile[speedRunProfile].endSpeed;
		case DS135L:  return curveLeft135DSProfile[speedRunProfile].endSpeed;
		case DS135R:  return curveRight135DSProfile[speedRunProfile].endSpeed;
		case DD90L:   return curveLeft90DDProfile[speedRunProfile].endSpeed;
		case DD90R:   return curveRight90DDProfile[speedRunProfile].endSpeed;
		default:			return 0;
	}
}
