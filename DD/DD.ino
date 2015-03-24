#include <Bridge.h>
#include <HttpClient.h>

#define HIGH 1
#define LOW 0
#define IN 1
#define OUT 0
#define InUltraPin 7
#define OutUlatraPin 6
#define InIrPin 2
#define OutIrPin 1
#define PirPin 2
#define detectRange 40
#define minConfidence 10
#define maxConfidence 90
#define TimeInterval 1000
#define TransitionTime 30000

int getIrDis(int sensorPin);
int getUltraDis(int sensorPin);
int pirReading();
void turnOnLed(int outputPin, int time);
void httpRequest(String DoorId, int transition, int confidence);

void setup() 
{
  Bridge.begin();
  Serial.begin(9600);
}

void loop() 
{

	int i = 0;
	int n = 0;
	int t = TransitionTime/TimeInterval;
	int transition = 0;
	int confidenceSum = 0;

	while ( i < t )
	{
		int ultraIn = getUltraDis(InUltraPin);
		int ultraOut = getUltraDis(OutUlatraPin);
		int irIn = getIrDis(InIrPin);
		int irOut = getIrDis(OutIrPin);

Serial.println(ultraIn);
Serial.println(ultraOut);
Serial.println(irIn );
Serial.println(irOut);
Serial.println("-------------");

		if ( ultraIn < detectRange && irIn < detectRange )
		{
			i = i + 1;
			n = n + 1;
			transition = transition + 1;
			
			if ((ultraIn + irIn) > 100)
			{
				confidenceSum = confidenceSum + minConfidence;
			}
			else if ((ultraIn + irIn) < 20)
			{
				confidenceSum = confidenceSum + maxConfidence;
			}
			else 
			{
				confidenceSum = confidenceSum + ((100 - (ultraIn + irIn))/100) * maxConfidence;
			}
			turnOnLed(12, TimeInterval/2);
			turnOnLed(12, TimeInterval/2);
		}

		if ( ultraOut < detectRange && irOut < detectRange )
		{
			i = i + 1;
			n = n + 1;
			transition = transition - 1;
			
			if ((ultraOut + irOut) > 100)
			{
				confidenceSum = confidenceSum + minConfidence;
			}
			else if ((ultraOut + irOut) < 20)
			{
				confidenceSum = confidenceSum + maxConfidence;
			}
			else 
			{
				confidenceSum = confidenceSum + ((100 - (ultraOut + irOut))/100) * maxConfidence;
			}
			turnOnLed(11, TimeInterval/2);
			turnOnLed(11, TimeInterval/2);
		}


		if ( ultraIn < detectRange && irIn > detectRange)
		{
			i = i + 1;
			n = n + 1;
			transition = transition + 1;
			
			if (ultraIn > 50)
			{
				confidenceSum = confidenceSum + minConfidence/2;
			}
			else if (ultraIn < 10)
			{
				confidenceSum = confidenceSum + maxConfidence/2;
			}
			else 
			{
				confidenceSum = confidenceSum + ((50 - ultraIn)/100) * maxConfidence;
			}
			turnOnLed(12, TimeInterval);
		}

		if ( ultraOut < detectRange && irOut > detectRange)
		{
			i = i + 1;
			n = n + 1;
			transition = transition - 1;
			
			if (ultraOut > 50)
			{
				confidenceSum = confidenceSum + minConfidence/2;
			}
			else if (ultraOut < 10)
			{
				confidenceSum = confidenceSum + maxConfidence/2;
			}
			else 
			{
				confidenceSum = confidenceSum + ((50 - ultraOut)/100) * maxConfidence;
			}
			turnOnLed(11, TimeInterval);
		}

		if ( irIn < detectRange && irIn > detectRange)
		{
			i = i + 1;
			n = n + 1;
			transition = transition + 1;
			
			if (irIn > 50)
			{
				confidenceSum = confidenceSum + minConfidence/2;
			}
			else if (irIn < 10)
			{
				confidenceSum = confidenceSum + maxConfidence/2;
			}
			else 
			{
				confidenceSum = confidenceSum + ((50 - irIn)/100) * maxConfidence;
			}
			turnOnLed(12, TimeInterval);
		}

		if ( irOut < detectRange && irOut > detectRange)
		{
			i = i + 1;
			n = n + 1;
			transition = transition - 1;
			
			if (irOut > 50)
			{
				confidenceSum = confidenceSum + minConfidence/2;
			}
			else if (irOut < 10)
			{
				confidenceSum = confidenceSum + maxConfidence/2;
			}
			else 
			{
				confidenceSum = confidenceSum + ((50 - irOut)/100) * maxConfidence;
			}
			turnOnLed(11, TimeInterval);
		}

		else
			{
				i = i + 1;
				delay(TimeInterval);
			}

	}

	String DoorId = "D1";
	int confidence = confidenceSum / n;
	httpRequest(DoorId, transition, confidence);

}


int getIrDis(int sensorPin)
{
	int irReading;
	int irInCm;
	pinMode(sensorPin,OUTPUT);

	irReading = analogRead(sensorPin);
	irInCm =(6762/(irReading-9))-4;

	if(irInCm > 80 | irInCm < 5)
	{
		irInCm = 100;
	}
	return irInCm;
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


void turnOnLed(int outputPin, int time)
{
	pinMode(outputPin, OUTPUT);

	digitalWrite(outputPin, HIGH);
	delay(time);
	digitalWrite(outputPin, LOW);
}


void httpRequest(String DoorId, int transition, int confidence)
{
	String URL = "http://qualoccupy.azurewebsites.net/add_data.php?DoorID=" 
				+ DoorId + "&transition=" + transition + "&Confidence=" 
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






















