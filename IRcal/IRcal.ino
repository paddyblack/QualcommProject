#include <DistanceGP2Y0A21YK.h>

DistanceGP2Y0A21YK DistIn;
DistanceGP2Y0A21YK DistOut;
int distanceIn, distanceOut;


void setup()
{
  Serial.begin(9600);
  DistIn.begin(A0);
  DistOut.begin(A1);
}

void loop()
{
  distanceIn = DistIn.getDistanceCentimeter();
  distanceOut = DistOut.getDistanceCentimeter();
 
 
  Serial.print(distanceIn); 
  Serial.print("\n"); 
  Serial.print(distanceOut); 
  Serial.print("\n\n\n\n\n"); 
  delay(2000); 
}
  
 
