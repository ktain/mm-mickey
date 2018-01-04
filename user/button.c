#include "main.h"

int32_t destX1 = 7;
int32_t destY1 = 7;

int32_t destX2 = 7;
int32_t destY2 = 8;

int32_t destX3 = 8;
int32_t destY3 = 7;

int32_t destX4 = 8;
int32_t destY4 = 8;

void button0(void) {
	delay_ms(100);
	
	switch(option) {
		case 0:
			delay_ms(2000);
		
			searchProfile = 0; 
			searchSpeed = 0.5;	// m/s
			turnSpeed = 0.5;	// m/s
			straightAccX = 2.0;	// m/s/s
			straightDecX = 2.0;	// m/s/s
		
			randomSearch();
			break;
		case 1:
			delay_ms(2000);
			
			searchProfile = 0; 
			searchSpeed = 0.5;	// m/s
			turnSpeed = 0.5;	// m/s
			traceSpeed = 0.7;	// Backtracking
			straightAccX = 2.0;	// m/s/s
			straightDecX = 2.0;	// m/s/s
		
			floodSearch(destX1, destY1, destX2, destY2, destX3, destY3, destX4, destY4);
			saveMaze();
			floodSearch(0, 0, 0, 0, 0, 0, 0, 0);
			saveMaze();
			break;
		case 2:
			delay_ms(2000);
			
			searchProfile = 0; 
			searchSpeed = 0.5;	// m/s
			turnSpeed = 0.5;	// m/s
			traceSpeed = 0.7;	// Backtracking
			straightAccX = 2.0;	// m/s/s
			straightDecX = 2.0;	// m/s/s
		
			floodSearchPivot(destX1, destY1, destX2, destY2, destX3, destY3, destX4, destY4);
			saveMaze();
			floodSearchPivot(0, 0, 0, 0, 0, 0, 0, 0);
			saveMaze();
			break;
		case 3:
			delay_ms(2000);
		
			searchProfile = 1; 
			speedRunProfile = 1;
		
			searchSpeed = 0.6;	// m/s
			turnSpeed = 0.6;	// m/s
			traceSpeed = 0.8;	// Backtracking
			straightSpeed = 1.1;	// Speed run
			straightAccX = 5.0;	// m/s/s
			straightDecX = 5.0;	// m/s/s

			loadMaze();
			speedRunClassic(destinationX, destinationY);
			floodSearch(0, 0, 0, 0, 0, 0, 0, 0);
			
			break;
		case 4:
			/*
			delay_ms(2000);
			
			searchProfile = 1; 
			speedRunProfile = 1;
		
			searchSpeed = 0.6;	// m/s
			turnSpeed = 0.6;	// m/s
			traceSpeed = 0.8;	// Backtracking
			straightSpeed = 0.7;	// Speed run
			straightAccX = 5.0;	// m/s/s
			straightDecX = 5.0;	// m/s/s
		
			enableMotorControl();
			moveStraight(13, 0.7, 0);
			delay_ms(200);
			disableMotorControl();
			*/
			calibrateIRSensors();
			break;
		default:
			break;
	}
	
	/*
	while (1) {
		printf("LF %d LD %d RD %d RF %d LENC %d RENC %d\r\n", LFSensor, LDSensor, RDSensor, RFSensor, getLeftEncCount(), getRightEncCount());
		delay_ms(100);
	}
	*/
	
}

void button1(void) {
	if (option == 0) {
		option = 1;
		LED1_ON;
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;
	}
	else if (option == 1) {
		option = 2;
		LED1_OFF;
		LED2_ON;
		LED3_OFF;
		LED4_OFF;
	}
	else if (option == 2) {
		option = 3;
		LED1_OFF;
		LED2_OFF;
		LED3_ON;
		LED4_OFF;
	}
	else if (option == 3) {
		option = 4;
		LED1_OFF;
		LED2_OFF;
		LED3_OFF;
		LED4_ON;
	}
	else if (option == 4) {
		option = 0;
		ALL_LED_OFF;
	}
	//while(1) {
	//	printf("LF %d LD %d RD %d RF %d LENC %d RENC %d\r\n", LFSensor, LDSensor, RDSensor, RFSensor, getLeftEncCount(), getRightEncCount());
	//	delay_ms(100);
	//}
}

