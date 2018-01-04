#include "main.h"

/* Sensor lookup tables */
int32_t sensorLookup[SENSOR_DATA_ARRAY_SIZE];
int32_t LFSensorLookup[SENSOR_DATA_ARRAY_SIZE];
int32_t RFSensorLookup[SENSOR_DATA_ARRAY_SIZE];
int32_t LDSensorLookup;
int32_t RDSensorLookup;

/* Sensor constants */
int32_t LDMidVal;
int32_t RDMidVal;
int32_t leftWallThreshold;	// when mouse is at half a cell distance
int32_t rightWallThreshold;
int32_t frontWallThresholdL;
int32_t frontWallThresholdR;
int32_t approachThreshold;
int32_t signalThreshold;
int32_t alignTime;
float alignScale;	// higher == lower pwm
int32_t alignErrorTolerance;

/* Sensor values updated every ms */
int32_t voltage;
volatile int32_t LFSensor;
volatile int32_t RFSensor;
volatile int32_t LDSensor;
volatile int32_t RDSensor;

/* Read IR Sensors */
void readIRSensors(void)
{
	
	//Read DC value		
	LFSensor = read_LF_Sensor;	
	//Read LF Sensor
	LF_EM_ON;
	delay_us(EMITTER_ON_TIME);
	LFSensor = read_LF_Sensor - LFSensor;
	LF_EM_OFF;

	if(LFSensor < 0) //error check
		LFSensor = 0;
	
	delay_us(EMITTER_OFF_TIME);
	
	// Read DC value
	RFSensor = read_RF_Sensor;	
	//Read RF Sensor
	RF_EM_ON;
	delay_us(EMITTER_ON_TIME);
	RFSensor = read_RF_Sensor - RFSensor;
	RF_EM_OFF;
	
	if(RFSensor < 0)
		RFSensor = 0;
 	delay_us(EMITTER_OFF_TIME);
	
	// Read DC value
	LDSensor = read_LD_Sensor;
	// Read LD Sensor
	LD_EM_ON;
	delay_us(EMITTER_ON_TIME);
	LDSensor = read_LD_Sensor - LDSensor;
	LD_EM_OFF;
	if(LDSensor < 0)
		LDSensor = 0;
	delay_us(EMITTER_OFF_TIME);
	
	// Read DC value
	RDSensor = read_RD_Sensor;	
	// Read RD Sensor
	RD_EM_ON;
	delay_us(EMITTER_ON_TIME);
	RDSensor = read_RD_Sensor - RDSensor;
	RD_EM_OFF;
	if(RDSensor < 0)
		RDSensor = 0;

}

// Returns sensor error 
int32_t getSensorError(void) {
	int32_t sensorError = 0;

	// Use front wall
	if (LFSensor > approachThreshold && RFSensor > approachThreshold) {
		for(int32_t i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
			if (LFSensorLookup[i] < LFSensor  || RFSensorLookup[i] < RFSensor) {
				sensorError = 2*((LFSensor - RFSensor) - (LFSensorLookup[i] - RFSensorLookup[i]));
				break;
			}
		}
	}
	
	// Otherwise use side walls
	else if (LDSensor > LDMidVal)
		sensorError = LDMidVal - LDSensor;
	else if (RDSensor > RDMidVal)
		sensorError = RDSensor - RDMidVal;
	
	// Positive sensor error turns to the left
	return sensorError;
}


/* Read voltage */
void readVoltage(void) {
	voltage = read_Voltage;
	if (voltage < LOWVOLTAGE) {
		ALL_LED_ON;
	}
}


/* Calibrate front sensors */
void calibrateIRSensors(void) {
	delay_ms(CALIBRATE_IR_SENSOR_START_DELAY);

	accX = calibrateAccX;
	decX = calibrateDecX;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;

	enableMotorControl();
	targetSpeedX = -mm_to_counts(calibrateSpeed);

	// Record raw values every mm
	for (int32_t i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
		LFSensorLookup[i] = LFSensor;
		RFSensorLookup[i] = RFSensor;
		while(-counts_to_mm((getLeftEncCount() + getRightEncCount())/2) < i) {
			if (i == 90) {
				LDSensorLookup = LDSensor + LDSensor/20;
				RDSensorLookup = RDSensor + RDSensor/20;
			}
		}
	}
	
	targetSpeedX = 0;
	
	// Single pass averaging of 2 most similar neighbors
	for (int32_t i = 2; i < SENSOR_DATA_ARRAY_SIZE-2; i++) {
		int32_t a, b, c, d, e, temp;
		
		// Left sensor
		a = LFSensorLookup[i-2];
		b = LFSensorLookup[i-1];
		c = LFSensorLookup[i];
		d = LFSensorLookup[i+1];
		e = LFSensorLookup[i+2];
		
		// Move lowest value to a
		if (b<a) {temp = a; a = b; b = temp;}
		if (c<a) {temp = a; a = c; c = temp;}
		if (d<a) {temp = a; a = d; d = temp;}
		if (e<a) {temp = a; a = e; e = temp;}
		
		// Move largest value to e
		if (b>e) {temp = e; e = b; b = temp;}
		if (c>e) {temp = e; e = c; c = temp;}
		if (d>e) {temp = e; e = d; d = temp;}
		
		// Average b, c, d
		LFSensorLookup[i] = (b + c + d)/3;
		
		// Repeat for right sensor
		a = RFSensorLookup[i-2];
		b = RFSensorLookup[i-1];
		c = RFSensorLookup[i];
		d = RFSensorLookup[i+1];
		e = RFSensorLookup[i+2];
		
		// Move lowest value to a
		if (b<a) {temp = a; a = b; b = temp;}
		if (c<a) {temp = a; a = c; c = temp;}
		if (d<a) {temp = a; a = d; d = temp;}
		if (e<a) {temp = a; a = e; e = temp;}
		
		// Move largest value to e
		if (b>e) {temp = e; e = b; b = temp;}
		if (c>e) {temp = e; e = c; c = temp;}
		if (d>e) {temp = e; e = d; d = temp;}
		
		// Average b, c, d
		RFSensorLookup[i] = (b + c + d)/3;
	}
	
	// Extrapolate first and last couple elements
	LFSensorLookup[0] = LFSensorLookup[1] = LFSensorLookup[2];
	RFSensorLookup[0] = RFSensorLookup[1] = RFSensorLookup[2];
	
	// Ensure values are monotonically decreasing
	for (int32_t i = 1; i < SENSOR_DATA_ARRAY_SIZE; i++) {
		if (LFSensorLookup[i] > LFSensorLookup[i-1]) 
			LFSensorLookup[i] = LFSensorLookup[i-1];
		if(RFSensorLookup[i] > RFSensorLookup[i-1])
			RFSensorLookup[i] = RFSensorLookup[i-1];
	}
	
	// Store average of left and right
	for (int32_t index = 0; index < SENSOR_DATA_ARRAY_SIZE; index++) {
		sensorLookup[index] = (LFSensorLookup[index] + RFSensorLookup[index])/2;
	}
	
	delay_ms(500);
	disableMotorControl();
	
	// Save sensor data to flash
	saveSensorData();

	// Load sensor data from flash
	loadSensorData();
}

