int outputPin = 2;
int lightPin = 13;
int counter = 0;

void setup()
{
  Serial.begin(9600);
  pinMode (outputPin,INPUT);
  pinMode (lightPin, OUTPUT);
}

void loop ()
{
  int sensorReading = digitalRead(outputPin);
  
  if (sensorReading == HIGH)
  {
    digitalWrite(lightPin,HIGH);
    counter = counter + 1;
    Serial.print("Number of people in the room: ");
    Serial.println(counter);
    delay(4000);
    digitalWrite(lightPin,LOW);
  }
  else
  {
    Serial.print("Number of people in the room: ");
    Serial.println(counter);
    delay(4000);
  }
}


