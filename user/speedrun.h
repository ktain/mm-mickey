#ifndef SPEEDRUN_H
#define SPEEDRUN_H

extern uint16_t nextDir[300];
extern int32_t length[300];

void speedRunClassic(uint16_t targetX, uint16_t targetY);
void speedRunDiag(uint16_t targetX, uint16_t targetY);
void generatePathNESW(uint16_t targetX, uint16_t targetY);
void generatePathSLR(uint16_t targetX, uint16_t targetY);
void generatePathDiag(uint16_t targetX, uint16_t targetY);
uint16_t getNextDirection(uint16_t xPos, uint16_t yPos);
void closeUntracedCells(void);
int32_t countDiagonals(uint32_t startDir, uint32_t head);

#define SS0 1
#define SS90L 2
#define SS90R 3
#define SS180L 4
#define SS180R 5
#define SD45L 6
#define SD45R 7
#define SD135L 8
#define SD135R 9
#define DS45L 10
#define DS45R 11
#define DS135L 12
#define DS135R 13
#define DD90L 14
#define DD90R 15
#define DD0 16
	
#endif

