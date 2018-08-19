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
#define INDEX_SIZE 48 // buffer size set to 48 char 
#define tag1 1
#define tag2 2

SoftwareSerial RFin(RX1,TX1);
SoftwareSerial DebugOut(RX2,TX2);

char rxbuffer[INDEX_SIZE] = {}; //  receive buffer
long ID=0;                   // 
int buffptr = 0; // position in circular buffer above

void setup()                    
{
   RFin.begin(SensorRate);
   DebugOut.begin(SensorRate);
   pinMode(ALERT,OUTPUT);
   digitalWrite(ALERT, HIGH);
}

void loop()                    
{
   buffptr=0;
   while(RFin.available()>0)
       {
        rxbuffer[buffptr]=RFin.read();
       }              // end while serial
   DebugOut.print("READ IN:\n");   
   for(int i=0;i<=buffptr;i++)
      {     
       DebugOut.print(rxbuffer[i],HEX);
      }
   DebugOut.write('\n');  
   ID=strtol(rxbuffer,NULL,16); 
   DebugOut.print(ID,DEC);
   DebugOut.write('\n');  
   if(ID==tag1 || ID==tag2)  // if authoried tag detected, 
      {
      digitalWrite(ALERT,LOW);
      delay(200);
      digitalWrite(ALERT,HIGH);
      }  
}


