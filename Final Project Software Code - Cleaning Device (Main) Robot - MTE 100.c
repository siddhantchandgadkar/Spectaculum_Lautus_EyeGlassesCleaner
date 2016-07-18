//Cleaning Device (Main) Robot - Final Project Software Code
//Himel Mondal, Rimpey Diaz, Siddhant Chandgadkar, Brandon Fong

#include "NXTServo-lib-UW.c"

/*Calibrate- Scans the bounds of the area of the robot
until a red starting point is found */
//Written by Himel Mondal
int calibrate(){
	time1[T1] = 0;
	bool hasReached = false;
	int stepTimer = 0;
	while(!hasReached){ //Motor movements looped in here until starting point found
		if(stepTimer > 5000){ //When x-direction bounds reached in step timer
			setServoSpeed(S1,2,-4,-15,12); //go back to original position
			wait1Msec(stepTimer);
			setServoSpeed(S1,2,0,-15,12);
			stepTimer = 0;
			wait1Msec(200);
		}
		setServoSpeed(S1,2,4,-15,12); //steps in x-direc for 1000ms
		wait1Msec(1000);
		stepTimer += 1000;
		setServoSpeed(S1,2,0,-15,12);
		wait1Msec(300);
		time1[T1] = 0;
		setServoSpeed(S1,1,-7, -11, 19);
		while(SensorValue[S3] != REDCOLOR && time1[T1] <= 2500){} // move in y-direction until redblock is found or 2500 ms pass
		setServoSpeed(S1,1,0, -11, 19);
		if(SensorValue[S3] == REDCOLOR) //calibrate ends if red is reached
		{
			hasReached = true;
		}
		else{
			setServoSpeed(S1,1,6, -11, 19); //moves back to original position in y-direction
			wait1Msec(2500);
			setServoSpeed(S1,1,0, -11, 19);
		}
	}
	return stepTimer;
}

/*Scans the x and y direction for dimensions of glasses indicated
by red blocks */
//Written by Siddhant Chandgadkar
int scan(int direc){ //takes in scan direction
	time1[T2] = 0;
	if(direc == 1){
		setServoSpeed(S1,1,-7, -11, 19); //will move forward until next red block reached in x - direc
		wait1Msec(3000);
		while(SensorValue[S3] != REDCOLOR){}
		setServoSpeed(S1, 1, 0, -11,19);
	}
	else if(direc == 2){ //will move sideways until next red block reached in y-direc
		setServoSpeed(S1,2,7, -15, 12);
		wait1Msec(3000);
		while(SensorValue[S3] != REDCOLOR){}
		setServoSpeed(S1, 2, 0, -15,12);
	}
	return time1[T2]; //returns the time it takes to move in either x or y direction
}

/* Communicates with the sprayer bot until it reaches the base */
//Written by Brandon Fong
bool bluetooth (){
	ubyte valueReceived[1];
	valueReceived[0] = 0;
	while(valueReceived[0] == 0){  //will search until a value from sprayer bot returned
		motor[motorB] = 4;	//keeps arms lifted until sprayed
		wait1Msec(800);
		motor[motorB] = 0;
		wait1Msec(5);
		cCmdMessageRead (valueReceived, 1, 2); //valueReceived indicates the current state of sprayer bot
		displayString(0,"%d",valueReceived[0]); //1 is the length of the array, 2 is the bluetooth channel
	}
	return true;
}

/* Returns the arms to its original calibrated position */
//Written by Rimpey Diaz
void originalPosition(int spot, int speed, int negOff, int posOff){ //takes in different motors and their values
	wait1Msec(1000);
	setServoSpeed(S1,spot,speed,negOff,posOff);
	wait1Msec(3000);
	while(SensorValue[S3] != REDCOLOR){} //Moves as specified in parameters until original red block reached
	setServoSpeed(S1,spot,0,negOff,posOff);
}

/* Sets the rotating arm in an optimal downward position for cleaning/scanning/etc. */
//Written by Brandon Fong
void setRotator(int &pos){
	while(SensorValue[S2] == 0){ //will spin until touch sensor pressed
		wait1Msec(1000);
		setServoPosition(S1, 5, pos);
		pos += 5;
	}
	setServoPosition(S1, 5, pos);
}

/* Well-named function that returns when the arm makes contact with cleaning arms */
//Written by Rimpey Diaz
bool doesMakeContact(int &pos){
	while(SensorValue[S4] == 0){ //moves glass clamp until touch sensor pressed
		setServoPosition(S1, 5, pos);
		wait1Msec(1000);
		pos-=5;
	}
	setServoPosition(S1, 4, pos);
	return (SensorValue[S4] == 1); //returns the state of the touch sensor
}

/*main cleaning function (arm pattern movement) */
//Written by Siddhant Chandgadkar
void traverse(int heightTime, int widthTime){ //Takes in dimensions from the scan functions
	for (int i = 0; i < 4; i++){
		setServoSpeed(S1,1,-7, -11,19); //for 4 iterations, moves the arm in a pattern in x, y direction
		motor[motorB] = -1;							//Movements based on scan dimension times
		wait1Msec(heightTime-300);
		setServoSpeed(S1,1,0, -11,19);
		motor[motorB] = 0;
		setServoSpeed(S1,2,-7, -15,12);
		wait1Msec(widthTime/4);
		setServoSpeed(S1,2,0, -15,12);
		setServoSpeed(S1,1,7,-11,19);
		motor[motorB] = 1;
		wait1Msec(heightTime-300);
		setServoSpeed(S1,1,0,-11,19);
		motor[motorB] = 0;
		wait1Msec(400);
	}
}

//Written by Himel Mondal
task main()
{
	/* Declarations */
	SensorType[S1] = sensorI2CCustom9V;
	SensorType[S3] = sensorColorNxtFULL;
	SensorType[S2] = sensorTouch;
	SensorType[S4] = sensorTouch;
	setServoSpeed(S1,2,0, -15, 12);
	setServoSpeed(S1,1,0, -11, 19);
	motor[motorB] = 0;
	nMotorEncoder[motorB] = 0;
	displayString(0,"%d", SensorValue[S2]);

	/* Sets rotators initial position */
	int pos = 0;
	setRotator(pos);
	displayString(0,"%d",pos);
	wait1Msec(1500);

	/* Moves arm up to be sprayed */
	motor[motorB] = 20;
	while(nMotorEncoder[motorB] < 90){}
	motor[motorB] = 0;

	/* calls bluetooth function */
	bool communication = bluetooth();
	displayString(0,"%s", communication);

	/*sets the arm that moves the cleaning arms back to original position*/
	motor[motorB] = 0;
	wait1Msec(2000);
	motor[motorB] = -7;
	wait1Msec(1000);
	motor[motorB] = 0;

	/* Calibrate function called */
	int step = calibrate();

	/* Height Scan */
	int hTime = scan(1);
	displayString(0,"%d", hTime);

	originalPosition(1,5,-11,19);

	/* Width Scan */
	int wTime = scan(2);
	displayString(0,"%d", wTime);

	originalPosition(2,-5,-15,12);

	wait1Msec(1000);

	/*Sets appropriate starting position to clean*/
  setServoSpeed(S1,1,-7,-11,19);
  wait1Msec(hTime/4);
  setServoSpeed(S1,1,0,-11,19);
  setServoSpeed(S1,2,7,-15,12);
  wait1Msec(wTime/4);
  setServoSpeed(S1,2,0,-15,12);

  /*Checks if rotating clamp makes contact with cleaning arms */
  bool contact = doesMakeContact(pos);
  displayString(0,"%s", contact);
  wait1Msec(3000);

  /* Traverse function called */
  if(contact){
  	traverse(hTime,wTime);
  }
  wait1Msec(500);

  /* sets the rotating clamp back to original position */
 	setRotator(pos);

 	/* Moves x and y direction motors back to their orignal position */
 	setServoSpeed(S1,2,-4,-15,12);
 	wait1Msec(step - (wTime/2));
 	setServoSpeed(S1,2,0,-15,12);

 	setServoSpeed(S1,1,6, -11, 19);
 	wait1Msec(2000);
 	setServoSpeed(S1,1,0, -11, 19);
}
