int duration;                                                        
int distance;
int counter = 0;
int sensorpin = 7;                                              

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  pinMode(sensorpin, OUTPUT);
  digitalWrite(sensorpin, LOW);                          
  delayMicroseconds(2);
  digitalWrite(sensorpin, HIGH);                         
  delayMicroseconds(10);
  digitalWrite(sensorpin, LOW);                            
  pinMode(sensorpin, INPUT);
  duration = pulseIn(sensorpin, HIGH);                        
  distance = duration/58;  
  if (distance < 30)
  {
    counter = counter + 1;
    Serial.print("Number of people in room:");
    Serial.println(counter);     
    delay(1000);   
  }
  else
  {
    Serial.print("Number of people in room:");
    Serial.println(counter); 
    delay(1000);
  }
}
