// This #include statement was automatically added by the Particle IDE.
#include "blynk/blynk.h"


char auth[] = "BLYNKAUTHKEYHERE";
int reedSwitch = D4; //input for the reed switch - detects open/closed status
bool current = 0; //logical placeholder - current status of the garage
bool previous = 1; //logical placeholder - previous status of the garage 
int seconds = 0;
int openlimit = 600; //number of seconds before text sent indicating garage open

void setup()
{
  Serial.begin(9600); //begin serial communication
  Blynk.begin(auth); //begin Blynk app communication
  pinMode(D0, OUTPUT); //initialize D0 as output pin - relay coil control
  pinMode(D4,INPUT); //initialize reedswitch pin as input
}  

void loop()
{
    Blynk.run(); //run Blynk app
    
    current = digitalRead(reedSwitch); //set current status open/closed
    if (current != previous){ //only run if there is a status change from previous state
        previous = current;   //reinitialize
        
        if (digitalRead(reedSwitch) == HIGH) { //is signal HIGH? i.e. garage is closed
            delay(500);
            //Spark.publish("garageStatus","closed",60,PRIVATE); //this line commented out as closed status not of interest
        }
        else {
        // Otherwise, this isn't a new status, and we don't have to do anything.
        }
      
        if (digitalRead(reedSwitch) == LOW) { //is signal LOW? i.e. garage is open
            delay(500);
            while (seconds < openlimit && digitalRead(reedSwitch) == LOW){ //while loop to publish only if garage is open more than ten minutes
                ++seconds;
                delay(1000);
                if (seconds ==599){
                    Spark.publish("garageStatus","open",60,PRIVATE); //send to spark publish which then triggers text via IFTTT
                    seconds = 0; 
                }
            }
        }
        else {
          // Otherwise, this isn't a new status, and we don't have to do anything.
        }
    delay(500);
    }
    
}