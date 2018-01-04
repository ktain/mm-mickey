#include "main.h"

bool showTrace;

/* 
 * Function name: printMaze()
 * Description: Prints the maze
 */
void printMaze(void) {
  for (int32_t y = SIZEY - 1; y >= 0; y--) {
		
    /*
     * Print north wall, mouse, and traces
     */
    for (int32_t x = 0; x < SIZEX; x++) {
      if ( hasNorthWall(x, y) )
        if (HIDEEAST)
          printf("+----");
        else 
          printf("+----+");
      else
        if (HIDEEAST)
          printf("+    ");
        else
          printf("+    +");
    }
    
    if (HIDEEAST) {
      printf("+");
    }
    
    printf("\n\r");
  
    
    
    /*
     * Print west and east wall, mouse, and traces
     */
    for (int32_t x = 0; x < SIZEX; x++) {
      
      // Print west wall
      if ( hasWestWall(x, y) )
        printf("|");
      else
        printf(" ");
      
      // Print if mouse present
      if ( x == curPosX && y == curPosY ) {
        if (orientation == NORTH)
          printf("  ^ ");
        else if (orientation == EAST)
          printf("  > ");
        else if (orientation == SOUTH)
          printf("  v ");
        else if (orientation == WEST)
          printf("  < ");
      }
      
      // Print markers
			else if (hasTrace(x, y) && showTrace) {
				printf("  . ");
			}
      else {
        printf("%4d", distance[x][y]);
      }
      
      // Opt to print east wall
      if (!HIDEEAST) {
        if ( hasEastWall(x, y) )
          printf("|");
        else
          printf(" ");
      }
    }
    
    // Print east boundary if necessary
    if (HIDEEAST){
      printf("|");
    }
    
    printf("\n\r");
    
    // Opt to print south wall
    if (!HIDESOUTH) {
      for (int32_t x = 0; x < SIZEX; x++) {
        if ( hasSouthWall(x, y) ) {
          if (HIDEEAST) 
            printf("+----");
          else
            printf("+----+");
        }
        else {
          if (HIDEEAST)
            printf("+    ");
          else 
            printf("+    +");
        }
      }
      
      if (HIDEEAST) {
        printf("+");
      }
      
      printf("\n\r");
    }
  }
  
    // Print south boundary if necessary 
    if(HIDESOUTH) {
      for(int32_t x = 0; x < SIZEX; x++) {
        if(HIDEEAST)
          printf("+----");
        else 
          printf("+----+");     
      }
      
      if (HIDEEAST) {
        printf("+");
      }
      
      printf("\n\r");
    }
  
}

void plotSensorData(void) {
	printf("{XYPLOT|SET|TITLE=Sensor Distances}");
	for (int32_t i = 0; i < SENSOR_DATA_ARRAY_SIZE ; i++) {
		printf("{XYPLOT|DATA|Left|%d|%d}", i, LFSensorLookup[i]);
		printf("{XYPLOT|DATA|Right|%d|%d}", i, RFSensorLookup[i]);
	}
	printf("\n\r");
}

void transmit(void) {
	while(1) {
		printf("{TABLE|SET|Voltage|%d}", voltage);
		printf("{TABLE|SET|LENC|%d}", getLeftEncCount());
		printf("{TABLE|SET|RENC|%d}", getRightEncCount());
		printf("{TABLE|SET|LFT|%d}", LFSensor);
		printf("{TABLE|SET|RFT|%d}", RFSensor);
		printf("{TABLE|SET|LD|%d}", LDSensor);
		printf("{TABLE|SET|RD|%d}", RDSensor);
	}
}
