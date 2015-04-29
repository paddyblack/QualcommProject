#define speakerPin 0
#define TimeIntervalInMs 1000
#define TransitionTimeInMs 3000

int getSoundLevel();

void setup() 
{
  Serial.begin(9600); 
}

void loop() 
{
  Serial.println(getSoundLevel());
  
}

int getSoundLevel()
{
  pinMode(speakerPin, INPUT);

  int i = 0;
  int max = 0;
  int min = 1024;
  int peakToPeak = 0;
  int s;

  while (i < TransitionTimeInMs/TimeIntervalInMs)
  {
    s = analogRead(speakerPin);
    i = i + 1;
    delay(TimeIntervalInMs);

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
 
