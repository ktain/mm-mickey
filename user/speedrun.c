#include "main.h"

uint16_t nextDir[300] = {0};
int32_t length[300] = {0};
int32_t tempDistance[SIZEX][SIZEY];
int32_t tempCell[SIZEX][SIZEY];

uint16_t pathSLR[300] = {0};
uint16_t path[300] = {0};

void speedRunClassic(uint16_t targetX, uint16_t targetY){
	generatePathNESW(targetX, targetY);
	enableMotorControl();

	for (uint32_t i = 0; length[i] != 0; i++) {
		// First straight
		if (i == 0) {
			if (curPosX == 0 && curPosY == 0 && orientation == NORTH)
				moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + length[i] - 0.5f, straightSpeed, turnSpeed);
			else
				moveStraight(length[i] - 0.5f, straightSpeed, turnSpeed);

			if (orientation == NORTH)
				curPosY += length[i];
			else if (orientation == EAST)
				curPosX += length[i];
			else if (orientation == SOUTH)
				curPosY -= length[i];
			else if (orientation == WEST)
				curPosX -= length[i];
		}
		// Last straight
		else if (length[i+1] == 0) {
			moveStraight(length[i] - 0.5f, straightSpeed, stopSpeed);
			if (orientation == NORTH)
				curPosY += length[i] - 1;
			else if (orientation == EAST)
				curPosX += length[i] - 1;
			else if (orientation == SOUTH)
				curPosY -= length[i] - 1;
			else if (orientation == WEST)
				curPosX -= length[i] - 1;
		}
		// Intermediate straight
		else {
			moveStraight(length[i] - 1, straightSpeed, turnSpeed);
			if (orientation == NORTH)
				curPosY += length[i] - 1;
			else if (orientation == EAST)
				curPosX += length[i] - 1;
			else if (orientation == SOUTH)
				curPosY -= length[i] - 1;
			else if (orientation == WEST)
				curPosX -= length[i] - 1;
		}

		if (length[i+1] == 0)
			break;

		// Curve turn
		if (nextDir[i] == NORTH)
			moveN();
		else if (nextDir[i] == EAST)
			moveE();
		else if (nextDir[i] == SOUTH)
			moveS();
		else if (nextDir[i] == WEST)
			moveW();
	}

	delay_ms(200);

	// If has front wall, align
	if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
	   (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
		align();
	}
	
	// If has left wall, align with left wall
	if (useAlignment && ((orientation == NORTH && hasWestWall(curPosX, curPosY)) || (orientation == EAST && hasNorthWall(curPosX, curPosY)) ||
	   (orientation == SOUTH && hasEastWall(curPosX, curPosY)) || (orientation == WEST && hasSouthWall(curPosX, curPosY)))) {
		pivotLeft90();
		align();
		pivotLeft90();
	}
	// Else if has right wall, align with right wall
	else if (useAlignment && ((orientation == NORTH && hasEastWall(curPosX, curPosY)) || (orientation == EAST && hasSouthWall(curPosX, curPosY)) ||
		 (orientation == SOUTH && hasWestWall(curPosX, curPosY)) || (orientation == WEST && hasNorthWall(curPosX, curPosY)))) {
		pivotRight90();
		align();
		pivotRight90();
	}
	// Else, no alignment, rotate right 180
	else {
		pivotRight180();
	}

	if (orientation == NORTH)
		orientation = SOUTH;
	else if (orientation == EAST)
		orientation = WEST;
	else if (orientation == SOUTH)
		orientation = NORTH;
	else if (orientation == WEST)
		orientation = EAST;

	disableMotorControl();
}

void generatePathNESW(uint16_t targetX, uint16_t targetY) {
	uint32_t count = 0;

	// zero nextDir and length array
	for (int32_t i = 0; i < 100; i++) {
		nextDir[i] = 0;
		length[i] = 0;
	}

	// Store copy of distance values
	for (uint32_t i = 0; i < SIZEX; i++) {
		for (uint32_t j = 0; j < SIZEY; j++) {
			tempDistance[i][j] = distance[i][j];
		}
	}

	// Store copy of wall info
	for (uint32_t i = 0; i < SIZEX; i++) {
		for (uint32_t j = 0; j < SIZEY; j++) {
			tempCell[i][j] = cell[i][j];
		}
	}

	// Simulate current position and orientation
	uint16_t simPosX = curPosX;
	uint16_t simPosY = curPosY;
	uint16_t simOrientation = orientation;

	// Block off untraced routes
	closeUntracedCells();
	updateDistancesComplete(targetX, targetY, targetX, targetY, targetX, targetY, targetX, targetY);

	uint32_t i;
	for (i = 0; !(simPosX == targetX && simPosY == targetY); i++) {
		if (simOrientation == NORTH) {
			while (!hasNorthWall(simPosX, simPosY) && (distance[simPosX][simPosY+1] == distance[simPosX][simPosY] - 1) && hasTrace(simPosX, simPosY+1)) {
				simPosY++;
				count++;
			}
		}
		else if (simOrientation == EAST) {
			while (!hasEastWall(simPosX, simPosY) && (distance[simPosX+1][simPosY] == distance[simPosX][simPosY] - 1) && hasTrace(simPosX+1, simPosY)) {
				simPosX++;
				count++;
			}
		}
		else if (simOrientation == SOUTH) {
			while (!hasSouthWall(simPosX, simPosY) && (distance[simPosX][simPosY-1] == distance[simPosX][simPosY] - 1) && hasTrace(simPosX, simPosY-1)) {
				simPosY--;
				count++;
			}
		}
		else if (simOrientation == WEST) {
			while (!hasWestWall(simPosX, simPosY) && (distance[simPosX-1][simPosY] == distance[simPosX][simPosY] - 1) && hasTrace(simPosX-1, simPosY)) {
				simPosX--;
				count++;
			}
		}
		//Error check
		if (count == 0) {
			// Restore distance values
			for (uint32_t i = 0; i < SIZEX; i++) {
				for (uint32_t j = 0; j < SIZEY; j++) {
					distance[i][j] = tempDistance[i][j];
				}
			}

			// Restore cell data
			for (uint32_t i = 0; i < SIZEX; i++) {
				for (uint32_t j = 0; j < SIZEY; j++) {
					cell[i][j] = tempCell[i][j];
				}
			}
			return;
		}
		length[i] = count;
		nextDir[i] = getNextDirection(simPosX, simPosY);
		simOrientation = nextDir[i];
		count = 0;
	}

	// Encode stopping condition
	length[i] = 0;
	nextDir[i] = 0;

	// Restore distance values
	for (uint32_t i = 0; i < SIZEX; i++) {
		for (uint32_t j = 0; j < SIZEY; j++) {
			distance[i][j] = tempDistance[i][j];
		}
	}

	// Restore cell data
	for (uint32_t i = 0; i < SIZEX; i++) {
		for (uint32_t j = 0; j < SIZEY; j++) {
			cell[i][j] = tempCell[i][j];
		}
	}
}

// Returns next direction to move in
uint16_t getNextDirection(uint16_t xPos, uint16_t yPos) {
	uint32_t curDist = distance[xPos][yPos];
	uint16_t nextDir = 0;
	uint32_t distN = MAX_DIST;
	uint32_t distE = MAX_DIST;
	uint32_t distS = MAX_DIST;
	uint32_t distW = MAX_DIST;
	
	if (yPos < SIZEY-1)
		distN = distance[xPos][yPos+1];
	if (xPos < SIZEX-1)
		distE = distance[xPos+1][yPos];
	if (yPos > 0) 
		distS = distance[xPos][yPos-1];
	if (xPos > 0)
		distW = distance[xPos-1][yPos];
	
	if (!hasNorthWall(xPos, yPos) && (distN == curDist-1)) {
		nextDir = NORTH;
	}
	else if (!hasEastWall(xPos, yPos) && (distE == curDist-1)) {
		nextDir = EAST;
	}
	else if (!hasSouthWall(xPos, yPos) && (distS == curDist-1)) {
		nextDir = SOUTH;
	}
	else if (!hasWestWall(xPos, yPos) && (distW == curDist-1)) {
		nextDir = WEST;
	}
	
	return nextDir;
}

void closeUntracedCells(void) {
	for (uint32_t x = 0; x < SIZEX; x++) {
		for (uint32_t y = 0; y < SIZEY; y++) {
			if (!hasTrace(x, y)) {
				placeWall(x, y, NORTH);
				placeWall(x, y, EAST);
				placeWall(x, y, SOUTH);
				placeWall(x, y, WEST);
			}
		}
	}
}

void generatePathSLR(uint16_t targetX, uint16_t targetY) {
	// Path translation
	generatePathNESW(targetX, targetY);

	// zero pathSLR array
	for (int32_t i = 0; i < 300; i++) {
		pathSLR[i] = 0;
	}

	uint32_t i, j;
	uint32_t head = 0;
	for (i = 0; length[i] != 0; i++) {

		// first case
		if (i == 0) {
			for (j = 0; j < length[i]; j++) {
				pathSLR[head + j] = STRAIGHT;
			}
			head += j;
			if (nextDir[i] == NORTH && orientation == EAST) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == NORTH && orientation == WEST) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == EAST && orientation == NORTH) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == EAST && orientation == SOUTH) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == SOUTH && orientation == EAST) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == SOUTH && orientation == WEST) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == WEST && orientation == NORTH) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == WEST && orientation == SOUTH) {
				pathSLR[head] = RIGHT;
			}
		}
		// intermediate case
		else {
			for (j = 0; j < length[i]-1; j++) {
				pathSLR[head + j] = STRAIGHT;
			}
			head += j;
			if (nextDir[i] == NORTH && nextDir[i-1] == EAST) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == NORTH && nextDir[i-1] == WEST) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == EAST && nextDir[i-1] == NORTH) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == EAST && nextDir[i-1] == SOUTH) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == SOUTH && nextDir[i-1] == EAST) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == SOUTH && nextDir[i-1] == WEST) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == WEST && nextDir[i-1] == NORTH) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == WEST && nextDir[i-1] == SOUTH) {
				pathSLR[head] = RIGHT;
			}
		}
		head++;
	}

	// last case
	head--;
	pathSLR[head] = STRAIGHT;
}


