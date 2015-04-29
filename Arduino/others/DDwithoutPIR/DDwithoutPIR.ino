#include <Bridge.h>
#include <HttpClient.h>

#define HIGH 1
#define LOW 0
#define IN 1
#define OUT 0
#define InPin 7
#define OutPin 6
#define InLed 12
#define OutLed 11

#define UltraRange 60
#define TimeIntervalInMs 1000
#define TransitionTimeInMs 30000

int getUltraDis(int sensorPin);
int motionDetect(int sensorPin, int distance);
int humanMotionDetect(int direction);
int getTransition();
void httpRequest();
void turnOnLed(int outputPin, int timeInMs);

void setup() 
{
  Bridge.begin();
  Serial.begin(9600);
  
  pinMode(InLed, OUTPUT);
  pinMode(OutLed, OUTPUT);
}

void loop() 
{
  httpRequest();
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



int humanMotionDetect(int direction)
{
	switch(direction)
	{
		case IN:
			if(motionDetect(InPin) == true)
			{
                                digitalWrite(InLed,HIGH);
				delay(TimeIntervalInMs);
                                digitalWrite(InLed,LOW);
				return true;
			}
			else 
			{
				return false;
			}

		case OUT:
			if(motionDetect(OutPin) == true)
			{
				digitalWrite(OutLed,HIGH);
				delay(TimeIntervalInMs);
                                digitalWrite(OutLed,LOW);
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

	while (i < t)
	{
		if(humanMotionDetect(IN) == true)
		{
                        digitalWrite(InLed,HIGH);
			transition = transition + 1;
			i = i + 1;
			Serial.print("transition= ");
                        Serial.println(transition);
                        delay(TimeIntervalInMs);
                        digitalWrite(InLed,LOW);
		}
		if(humanMotionDetect(OUT) == true)
		{
                        digitalWrite(OutLed,HIGH);
			transition = transition - 1;
			i = i + 1;
			Serial.print("transition= ");
                        Serial.println(transition);
                        delay(TimeIntervalInMs);
                        digitalWrite(OutLed,LOW);

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
		Serial.print("HTTP Request: ");
		Serial.println(URL);
		turnOnLed(13,1000);
	}
}
