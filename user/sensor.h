#ifndef SENSOR
#define SENSOR

extern int32_t sensorLookup[SENSOR_DATA_ARRAY_SIZE];
extern int32_t LFSensorLookup[SENSOR_DATA_ARRAY_SIZE];
extern int32_t RFSensorLookup[SENSOR_DATA_ARRAY_SIZE];
extern int32_t LDSensorLookup;
extern int32_t RDSensorLookup;

extern int32_t voltage;
extern volatile int32_t LFSensor;
extern volatile int32_t RFSensor;
extern volatile int32_t LDSensor;
extern volatile int32_t RDSensor;
extern float sensorScale;
extern int32_t alignErrorTolerance;

extern int32_t LDMidVal;
extern int32_t RDMidVal;
extern int32_t leftWallThreshold;
extern int32_t rightWallThreshold;
extern int32_t frontWallThresholdL;
extern int32_t frontWallThresholdR;
extern int32_t approachThreshold;
extern int32_t signalThreshold;

extern int32_t alignTime;
extern float alignScale;

void readVoltage(void);
void readIRSensors(void);
int32_t getSensorError(void);
void calibrateIRSensors(void);

#endif
