#include <Bridge.h>
#include <HttpClient.h>

#define HIGH 1
#define LOW 0
#define PirPin1 1
#define PirPin2 2
#define speakerPin A0
#define TimeInterval 1000
#define TransitionTime 30000

int pirReading(int sensorPin);
int getSoundLevel();
void httpRequest(String DoorId, int transition, int confidence);

void setup() 
{
 	Serial.begin(9600); 
}

void loop() 
{
 
  if (pirReading(PirPin1) == true | pirReading(PirPin2) == true)
  {
    String DoorId = "D1";
    int transition = 1;
    int confidence = getSoundLevel();
    httpRequest(DoorId, transition, confidence);
  }
	
}

int pirReading(int sensorPin)
{
	pinMode(sensorPin, INPUT);

	if (digitalRead(sensorPin) == HIGH)
	{
		return true;
	}
	if (digitalRead(sensorPin) == LOW)
	{
		return false;
	}
}

int getSoundLevel()
{
  pinMode(speakerPin, INPUT);

  int i = 0;
  int max = 0;
  int min = 1024;
  int peakToPeak = 0;
  int s;

  while (i < TransitionTime/TimeInterval)
  {
    s = analogRead(speakerPin);
    i = i + 1;
    delay(TimeInterval);

    if (s < 1024)
    {
      if (s > max)
      {
        max = s;
      }
      else if (s < min)
      {
        min = s;
      }
    }
  }

  peakToPeak = max - min;
  double level = (peakToPeak * 3.3)/ 10.24;

  if (level < 0)
  {
    level = level * -1;
  }
  if (level > 100)
  {
    level = 100;
  }

  return level;
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
  String URL = "http://qualoccupy.azurewebsites.net/add_data_rooms.php?DoorID=" 
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
