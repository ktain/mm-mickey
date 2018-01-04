#include "main.h"

int32_t option = 0;

void systick(void) {
	if (useMotorControl)
		updateSpeed();
	
	readVoltage();
	readIRSensors();
}


int main(void) {
	loadSettings();	// Load settings from flash
	setup();	// Setup pin configurations
	initializeMaze();
	delay_ms(200);	// Allow voltage to settle after power on
	
	while(1) {
		//printf("LF %d LD %d RD %d RF %d LDMid %d RDMid %d\r\n", LFSensor, LDSensor, RDSensor, RFSensor, LDMidVal, RDMidVal);
		delay_ms(100);
	}
}
