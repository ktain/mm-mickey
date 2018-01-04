#include "main.h"

/* Move at random */
void randomSearch(void) {
	curPosX = 0;
	curPosY = 0;
	orientation = NORTH;
	
	enableMotorControl();
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + .5f, turnSpeed, turnSpeed);
	curPosY++;
	int32_t choice;
	
	while(1) {
		choice = micros()%3;
		
		// move forward
		if (choice == 0 && LFSensor < frontWallThresholdL && RFSensor < frontWallThresholdR) {
			if (orientation == NORTH && !(curPosX == 0 && curPosY+1 == 0)) {
				moveStraight(1, turnSpeed, turnSpeed);
				curPosY++;
			}
			else if (orientation == EAST && !(curPosX+1 == 0 && curPosY == 0)) {
				moveStraight(1, turnSpeed, turnSpeed);
				curPosX++;
			}
			else if (orientation == SOUTH && !(curPosX == 0 && curPosY-1 == 0)) {
				moveStraight(1, turnSpeed, turnSpeed);
				curPosY--;
			}
			else if (orientation == WEST && !(curPosX-1 == 0 && curPosY == 0)) {
				moveStraight(1, turnSpeed, turnSpeed);
				curPosX--;
			}
			else {
				moveBack();
				if (orientation == NORTH) {
					curPosY--;
					orientation = SOUTH;
				}
				else if (orientation == EAST) {
					curPosX--;
					orientation = WEST;
				}
				else if (orientation == SOUTH) {
					curPosY++;
					orientation = NORTH;
				}
				else if (orientation == WEST) {
					curPosX++;
					orientation = EAST;
				}
			}
		}
		// curve left
		else if (choice == 1 && LDSensor < leftWallThreshold) {
			if (orientation == NORTH && !(curPosX-1 == 0 && curPosY == 0)) {
				curveLeft90();
				curPosX--;
				orientation = WEST;
			}
			else if (orientation == EAST && !(curPosX == 0 && curPosY+1 == 0)) {
				curveLeft90();
				curPosY++;
				orientation = NORTH;
			}
			else if (orientation == SOUTH && !(curPosX+1 == 0 && curPosY == 0)) {
				curveLeft90();
				curPosX++;
				orientation = EAST;
			}
			else if (orientation == WEST && !(curPosX == 0 && curPosY-1 == 0)) {
				curveLeft90();
				curPosY--;
				orientation = SOUTH;
			}
			else {
				moveBack();
				if (orientation == NORTH) {
					curPosY--;
					orientation = SOUTH;
				}
				else if (orientation == EAST) {
					curPosX--;
					orientation = WEST;
				}
				else if (orientation == SOUTH) {
					curPosY++;
					orientation = NORTH;
				}
				else if (orientation == WEST) {
					curPosX++;
					orientation = EAST;
				}
			}
		}
		// curve right
		else if (choice == 2 && RDSensor < rightWallThreshold) {
			if (orientation == NORTH && !(curPosX+1 == 0 && curPosY == 0)) {
				curveRight90();
				curPosX++;
				orientation = EAST;
			}
			else if (orientation == EAST && !(curPosX == 0 && curPosY-1 == 0)) {
				curveRight90();
				curPosY--;
				orientation = SOUTH;
			}
			else if (orientation == SOUTH && !(curPosX-1 == 0 && curPosY == 0)) {
				curveRight90();
				curPosX--;
				orientation = WEST;
			}
			else if (orientation == WEST && !(curPosX == 0 && curPosY+1 == 0)) {
				curveRight90();
				curPosY++;
				orientation = NORTH;
			}
			else {
				moveBack();
				if (orientation == NORTH) {
					curPosY--;
					orientation = SOUTH;
				}
				else if (orientation == EAST) {
					curPosX--;
					orientation = WEST;
				}
				else if (orientation == SOUTH) {
					curPosY++;
					orientation = NORTH;
				}
				else if (orientation == WEST) {
					curPosX++;
					orientation = EAST;
				}
			}
		}
		else if (LFSensor > frontWallThresholdL && RFSensor > frontWallThresholdR && LDSensor > leftWallThreshold && RDSensor > rightWallThreshold) {
			// deadend
			moveBack();
			if (orientation == NORTH) {
				curPosY--;
				orientation = SOUTH;
			}
			else if (orientation == EAST) {
				curPosX--;
				orientation = WEST;
			}
			else if (orientation == SOUTH) {
				curPosY++;
				orientation = NORTH;
			}
			else if (orientation == WEST) {
				curPosX++;
				orientation = EAST;
			}
		}
	}
}
