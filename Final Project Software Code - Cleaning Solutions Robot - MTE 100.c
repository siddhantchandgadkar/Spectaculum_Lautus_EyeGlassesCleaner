//Cleaning Solution Robot - Final Project Software Code
//Himel Mondal, Rimpey Diaz, Siddhant Chandgadkar, Brandon Fong

//Written by Siddhant Chandgadkar
task main()
{
	SensorType[S1] = sensorSONAR;
	motor[motorA] = 40;
	while(SensorValue[S1]>8){}//moves forward until 8cm away from base
	motor[motorA] = 0;
	wait1Msec(60000);  //1 minute prep time for spraying glasses
	ubyte valueToSend[1];
	valueToSend[0] = 1;
	cCmdMessageWriteToBluetooth(valueToSend,1,2); //sends 1 to base to indicate it can continue cleaning
}
