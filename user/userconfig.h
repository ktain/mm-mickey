#ifndef USERCONFIG_H
#define USERCONFIG_H

// Maze Settings
#define SIZEX 16      					// size of maze
#define SIZEY 16
#define MAX_DIST (SIZEX*SIZEY)  // max distance for flood search

// Calibration settings
#define SENSOR_DATA_ARRAY_SIZE 300 						// 1mm per element
#define CALIBRATE_IR_SENSOR_START_DELAY 3000 	// Time in ms before starting

// Alignment settings
#define ANGULAR_CORRECTION_TIME 200			// Time in ms

// IR Sensor settings
#define EMITTER_ON_TIME 70			// Time in us
#define EMITTER_OFF_TIME 70			// Total time = 4*ON_TIME + 3*OFF_TIME

// Motor settings
#define MOTORPERIOD 999
#define MOTORPRESCALER 3 				// PWM frequency = 84000000 / (period+1) / (prescaler+1)
#define USESLOWDECAY 0					// 0 for fast decay

// USART settings
#define BAUDRATE 9600

// Voltage settings
#define VOLTAGEMULTIPLIER 18810/4096	// raw/4096 * 3300 * 5.7
#define LOWVOLTAGE 7000

// Print settings
#define HIDESOUTH 1  // Hide south cell wall when printing
#define HIDEEAST 1   // Hide east cell wall when printing

void loadSettings (void);

#endif
