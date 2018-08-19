#include <SoftwareSerial.h>
/* if RF reader detects an authorised tag, fire door control solenoid
 *  RF reader outputs ID of tag read onto serial port
 * Attiny85 uses D0,D2 (chip pins 5,7) for rf reader soft serial port, 
 * D4,D3 (chip pins 3,2) for debug/control port, and D1 (chip pin 6) for digital LOW alert
 */
#define RX1 3 // *** D3, Pin 2  
#define TX1 4 // *** D4, Pin 3 
#define RX2 0 // *** D0, Pin 5  
#define TX2 2 // *** D2, Pin 7  
#define ALERT 1  // digital out pin for threshold alert (D1, chip pin 6) 
#define DELAY 100    // delay in millisecs between cap tests
#define SensorRate 9600
#define INDEX_SIZE 48 // array of 48 char 
#define tag1 1
#define tag2 2

SoftwareSerial RFin(RX1,TX1);
SoftwareSerial DebugOut(RX2,TX2);

float rxbuffer[INDEX_SIZE] = {}; //  receive buffer
int ID=0;                   // 
int buffstart, bufftop = 0; // position in circular buffer above

void setup()                    
{
   RFin.begin(SensorRate);
   DebugOut.begin(SensorRate);
   pinMode(ALERT,OUTPUT);
   digitalWrite(ALERT, HIGH);
}

void loop()                    
{
   buffstart=bufftop;
   while(RFin.available()>0)
       {
        rxbuffer[bufftop]=RFin.read();
        // move slot in circular buffer
        if ( ++bufftop > INDEX_SIZE-1)  bufftop=0;
       }              // end while serial
   DebugOut.print("READ IN:\n");   
   for(int i=buffstart;i<bufftop;i++)
      {     
       DebugOut.print(rxbuffer[i],DEC);
      }
   DebugOut.write('\n');   
   if(ID==tag1 || ID==tag2)  // if authoried tag detected, 
      {
      digitalWrite(ALERT,LOW);
      delay(200);
      digitalWrite(ALERT,HIGH);
      }  
}


