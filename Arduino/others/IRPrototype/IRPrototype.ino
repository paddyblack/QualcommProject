#include <DistanceGP2Y0A21YK.h>

int pinRed = 12;
int pinGreen = 13;
DistanceGP2Y0A21YK DistIn;
DistanceGP2Y0A21YK DistOut;
int distanceIn, distanceOut;
int counter = 0;

void setup()
{
  Serial.begin(9600);
  DistIn.begin(A0);
  DistOut.begin(A1);
  pinMode(pinRed,OUTPUT);
  pinMode(pinGreen,OUTPUT);
}
 
void loop()
{
  distanceIn = DistIn.getDistanceCentimeter();
  distanceOut = DistOut.getDistanceCentimeter();
  if (distanceIn < 8)
  {
    digitalWrite(pinGreen,HIGH);
    counter = counter + 1;
    Serial.print("number of peopel in room: ");
    Serial.print(counter); 
    Serial.print("\n"); 
    delay(2000); 
    digitalWrite(pinGreen,LOW);
  }

  if (distanceOut < 8)
  {
    digitalWrite(pinRed,HIGH);
    if (counter - 1 > -1)
    counter = counter - 1;
    else counter = 0;
    Serial.print("number of peopel in room: ");
    Serial.print(counter); 
    Serial.print("\n"); 
    delay(2000); 
    digitalWrite(pinRed,LOW);
  }
 
}
