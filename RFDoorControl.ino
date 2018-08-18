#include <SoftwareSerial.h>
/*  Arduino Capacitive Sensor with serial interface for threshold setting   14/8/18   Bruce Woolmore
 * send "|" for command mode, then decimal value of next char * 10 = threshold for alert (sets alert pin, sends value to serial port)
 * Attiny85 uses D0,D2 (chip pins 5,7) for soft serial, D4,D3 (chip pins 3,2) for cap sense send/sense pins, D1 (chip pin 6) for digital LOW alert
 */
#define RX1 3 // *** D3, Pin 2  
#define TX1 4 // *** D4, Pin 3 
#define RX2 0 // *** D0, Pin 5  
#define TX2 2 // *** D2, Pin 7  
#define ALERT 1  // digital out pin for threshold alert (D1, chip pin 6) 
#define DELAY 100    // delay in millisecs between cap tests
#define SensorRate 9600
#define INDEX_SIZE 48 // array of 48 char 

SoftwareSerial RFin(RX1,TX1);
SoftwareSerial DebugOut(RX2,TX2);

bool cmdMode=false;
int Threshold=500; 
int Inbyte;
float press_array[INDEX_SIZE] = {0.0}; //  pressure readings
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
   while(RFin.available()>0)
       {
        Inbyte=RFin.read();
          // move slot in circular buffer
        if ( ++buffptr > INDEX_SIZE-1)  buffptr=0;
       }              // end while serial
       DebugOut.print("READ IN:\n");DebugOut.write(Inbyte);DebugOut.write(':');DebugOut.print(Inbyte,DEC);DebugOut.write('\n');
        DebugOut.write('\n');
        digitalWrite(ALERT,LOW);
        delay(200);
        digitalWrite(ALERT,HIGH);
}


