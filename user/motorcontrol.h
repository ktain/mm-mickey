#ifndef PWM_H
#define PWM_H   

struct turnProfile {
	int32_t d1;
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t d2;
	float startSpeed;
	float speedX;
	float endSpeed;
	int32_t speedW;
	int32_t accW;
	int32_t decW;
	float kpW13;
	float kdW13;
	float kpW2;
	float kdW2;
};

extern bool speedRunProfile;
extern bool searchProfile;

extern bool useMotorControl;
extern bool useIRSensors;
extern bool useAlignment;
extern bool useFrontWallCalibration;

extern float maxPwm;
extern float alignSpeed;
extern float calibrateSpeed;
extern float centerToBackDist;	// Distance in mm from center of mass to back
extern float cellDistance;
extern float frontEdgeDist;		// Distance in mm from front wall to front edge 
extern float frontEdgeDistTurn;
extern float counts_per_360;
extern float wheelDistance;
extern float alignSensorScale;
extern float leftPwmTotal;
extern float rightPwmTotal;

extern float encCountLeft;
extern float encCountRight;
extern float encCount;
extern float oldEncCountLeft;
extern float oldEncCountRight;
extern float encChangeLeft;
extern float encChangeRight;
extern float encChange;
extern float distanceLeft;

extern float targetSpeedX;
extern float targetSpeedW;
extern float curSpeedX;			
extern float curSpeedW;
extern float encFeedbackX;
extern float encFeedbackW;
extern float gyroFeedback;
extern float errorX;
extern float errorW;
extern float oldErrorX;
extern float oldErrorW;

extern float kpX;
extern float kdX;
extern float kpW;
extern float kdW;

extern float stopSpeed;
extern float searchSpeed;
extern float turnSpeed;
extern float traceSpeed;
extern float straightSpeed;
extern float diagSpeed;

extern float accX;
extern float decX;
extern float accW;
extern float decW;
extern float alignAccX;
extern float alignDecX;
extern float alignAccW;
extern float alignDecW;
extern float alignKdW;
extern float alignKpW;
extern float calibrateAccX;
extern float calibrateDecX;
extern float straightAccX;
extern float straightDecX;
extern float straightAccW;
extern float straightDecW;
extern float straightKdX;
extern float straightKpX;
extern float straightKdW;
extern float straightKpW;
	
void updateSpeed(void);
float getDecNeeded(int32_t d, float Vi, float Vf);
void setLeftPwm(int32_t pwm);
void setRightPwm(int32_t pwm);
void enableMotorControl(void);
void disableMotorControl(void);
void resetMotorParameters(void);

void moveStraight(float cells, float maxSpeed, float endSpeed);
void turn(struct turnProfile profile);
void align(void);
void calibrateTurn(float maxSpeed, float endSpeed);
void hugFrontWall(int32_t LSensorVal, int32_t RSensorVal);

float counts_to_mm(float counts);
float mm_to_counts(float speed);
float counts_to_deg(float counts);
float deg_to_counts(float deg);

extern int32_t hugTime;
extern int32_t hugPwm;
extern float hugSensorScale;
#endif
