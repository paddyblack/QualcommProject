#include <Bridge.h>
#include <HttpClient.h>

#define HIGH 1
#define LOW 0
#define IN 1
#define OUT 0
#define InPin 7
#define OutPin 6
#define PirPin 2
#define UltraRange 10
#define TimeIntervalInMs 1000
#define TransitionTimeInMs 30000

int getUltraDis(int sensorPin);
int motionDetect(int sensorPin, int distance);
int pirReading();
int humanMotionDetect(int direction);
int getTransition();
void httpRequest();
void turnOnLed(int outputPin, int timeInMs);

void setup() 
{
  Bridge.begin();
  Serial.begin(9600);
}

void loop() 
{
	Serial.println(getTransition());
}



int getUltraDis(int sensorPin)
{
	int duration;
	pinMode(sensorPin, OUTPUT);

	digitalWrite(sensorPin, LOW);                          
	delayMicroseconds(2);
	digitalWrite(sensorPin, HIGH);                         
	delayMicroseconds(10);
	digitalWrite(sensorPin, LOW);                            
	pinMode(sensorPin, INPUT);
	duration = pulseIn(sensorPin, HIGH);                        
	return duration/58;  
}

int motionDetect(int sensorPin)
{
	if (getUltraDis(sensorPin) < UltraRange)
	{
		return true;
	}
	else
	{
		return false;
	}
}


int pirReading()
{
	pinMode(PirPin, INPUT);

	if (digitalRead(PirPin) == HIGH)
	{
		return true;
	}
	if (digitalRead(PirPin) == LOW)
	{
		return false;
	}
}

int humanMotionDetect(int direction)
{
	switch(direction)
	{
		case IN:
			if(pirReading() == true && motionDetect(InPin) == true)
			{
				delay(TimeIntervalInMs);
				return true;
			}
			else 
			{
				return false;
			}

		case OUT:
			if(pirReading() == true && motionDetect(OutPin) == true)
			{
				delay(TimeIntervalInMs);
				return true;
			}
			else 
			{
				return false;
			}
	}
}

int getTransition()
{
	int i = 0;
 	int transition = 0;
	int t = TransitionTimeInMs/TimeIntervalInMs;
 `
	while (i < t)
	{
		if(humanMotionDetect(IN) == true)
		{
			transition = transition + 1;
			i = i + 1;
			Serial.println("transition= " + transition);
		}
		if(humanMotionDetect(OUT) == true)
		{
			transition = transition - 1;
			i = i + 1;
			Serial.println("transition= " + transition);
		}
		else
		{
			delay(TimeIntervalInMs);
			i = i + 1;
		}
	}

	return transition;
}

void turnOnLed(int outputPin, int timeInMs)
{
	pinMode(outputPin, OUTPUT);

	digitalWrite(outputPin, HIGH);
	delay(timeInMs);
	digitalWrite(outputPin, LOW);
}


void httpRequest()
{
	String DoorID = "D1";
	int confidence = 91;
	int transition = getTransition();
	String URL = "http://qualoccupy.azurewebsites.net/add_data.php?DoorID=" 
				+ DoorID + "&transition=" + transition + "&Confidence=" 
				+ confidence + "&event_time=NOW();";
	HttpClient client;
	if (transition != 0)
	{
		client.get(URL);
		Serial.println("HTTP Request: ");
		Serial.println(URL);
		turnOnLed(13,1000);
	}
}






















