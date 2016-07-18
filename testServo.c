#include "NXTServo-lib-UW.c"
task main()
{
	SensorType[S1] = sensorI2CCustom9V;
	//setDemoMotor(2,6);
	setServoSpeed(S1,1, 0, -10, 19);
	while(nNxtButtonPressed == -1){}
	while(nNxtButtonPressed != -1){}
	setServoSpeed(S1,1, 10, -10, 19);
	//while(nNxtButtonPressed == -1){}
	wait1Msec(1500);
	setServoSpeed(S1,1,0,-10,19);
	//setServoPosition(s1,2,40);
}
