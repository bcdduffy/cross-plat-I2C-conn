#include <Wire.h>

#define SLAVE_ADDRESS 0x48
#define BAUDRATE 9600

void setup() {
  // put your setup code here, to run once:
    Wire.begin(SLAVE_ADDRESS);
    Serial.begin(BAUDRATE);
    Wire.onReceive(Received);
    Wire.onRequest(Requested);
}

void loop() {
  // put your main code here, to run repeatedly:

}






//On a Master WRITE
void Received(int numBytes)
{
}
//On a Master READ
void Requested()
{
  Wire.write(0x52);
}
