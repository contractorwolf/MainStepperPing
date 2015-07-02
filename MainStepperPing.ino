#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(1, 3, 2);

int pos = 400;
int lastMeasuredPosition = 0;
int measureCount = 0;


const int pingPin = 7;
unsigned long old_time;
unsigned long new_time;
int distance;


bool direction = true;

void setup()
{  

	int speed = 10000;
	measureCount = 0;

	stepper.setMaxSpeed(speed);
	stepper.setAcceleration(speed);
	Serial.begin(9600);

	lastMeasuredPosition = stepper.currentPosition();

	old_time = millis();
	new_time = millis();
}

void loop()
{

	int measurementsNeeded = 12;
	int maxSteps = 800;
	int shiftAmount = (int)(maxSteps/measurementsNeeded);



	int currentPosition = stepper.currentPosition();

	if(direction){
		
		
		if(currentPosition<maxSteps){
			stepper.move(shiftAmount);

		}else{
			direction = false;

			//Serial.print(new_time - old_time);
			//Serial.print(" millis");
			//Serial.println();
			//old_time = new_time;
			//new_time = millis();


			measureCount = 0;


		}


	}else{
		if(currentPosition>0){
			stepper.move(-shiftAmount);

		}else{
			direction = true;
			measureCount = 0;

			//report on full pan time
			//Serial.print(new_time - old_time);
			//Serial.print(" millis");
			//Serial.println();
			//old_time = new_time;
			//new_time = millis();

		}
	}
	

	if(abs(currentPosition - lastMeasuredPosition)>shiftAmount){
		distance = Measure(currentPosition);
		
		//report measurement data
		Serial.print(currentPosition);
		Serial.print(",");
		Serial.print(distance);
		Serial.println("*");


		lastMeasuredPosition = currentPosition;
		measureCount++;

	}


	


	stepper.run();

	//delay(100);
}



long Measure(long position){
	//new_time = millis();

	// establish variables for duration of the ping, 
	// and the distance result in inches and centimeters:
	long duration, inches, cm;

	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(pingPin, OUTPUT);
	digitalWrite(pingPin, LOW);
	delayMicroseconds(2);
	digitalWrite(pingPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(pingPin, LOW);

	// The same pin is used to read the signal from the PING))): a HIGH
	// pulse whose duration is the time (in microseconds) from the sending
	// of the ping to the reception of its echo off of an object.
	pinMode(pingPin, INPUT);
	duration = pulseIn(pingPin, HIGH);

	// convert the time into a distance
	//inches = microsecondsToInches(duration);
	cm = microsecondsToCentimeters(duration);
  
	//Serial.print(inches);
	//Serial.print("in, ");
	//Serial.print(cm);
	//Serial.print("cm at ");
	//Serial.print(position);
	//Serial.print(" in ");
	//Serial.print(new_time - old_time);
	//Serial.print(" millis");
	//Serial.println();
	//old_time = new_time;



	return(cm);

}


long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
