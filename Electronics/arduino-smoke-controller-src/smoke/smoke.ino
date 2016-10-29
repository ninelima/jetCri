//-----------------------------------------------------------------------------
// PBS TJ20 Smoke controller
// (c) BVH Aviation 2016
//
// Author Dawie Botes 2016
// Date 2015-10-27
// Version 1.00 - Initial release
//

#include <Servo.h> 

//#define __DEBUGCODE


#define BUT_PIN 2
// attaches the servo on pin 11 to the servo object (PB3 MOSI PWM) // Duemlanove
#define PWM_PIN 9
// attaches the servo on pin 16 to the servo object (PB2 MOSI PWM) // Sparkfun Pro Micro
// #define PWM_PIN 16

#define POT_WIPER 2


Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
//-------------------------------------
// Zero sweep, PWM = 0.0 ms
// 
void smokeOff()
{
  myservo.write(0);  // 0 deg
}

//-------------------------------------
// Full sweep, PWM = 2000 ms
// 
void smokeOn()
{
  myservo.write(180);  // 180 deg
}


//-------------------------------------
// Read the STOP button
// true for pushed
// false for not pushed
//
bool readSwitch()
{
	if (digitalRead(BUT_PIN) == LOW) {
		//
    // switch is pressed - pullup keeps pin high normally
    //
    delay(100);    						  					// delay 100 ms to debounce switch
  	if (digitalRead(BUT_PIN) == LOW)      // the pin is still low after the delay 
  	  return true;                        // consider it pressed and return true  
  }
  return false;                           // button not pressed return false
}

//-------------------------------------
// Setup
//
void setup() 
{ 
	#ifdef __DEBUGCODE
	Serial.begin(9600);              // set baud rate for USB port
	Serial.println("PBS-TJ20A Smoke controller");
	#endif //__DEBUGCODE

	pinMode(BUT_PIN,  INPUT_PULLUP);  	// Button pin, internal 50K pullup enabled
	// pinMode(13, OUTPUT);           	// LED pin, cannot be used input without a pullodown beacause of LED
  
	myservo.attach(9); 									// attaches the servo on pin 11 to the servo object (PB3 MOSI PWM) // Duemlanove
	//myservo.attach(16);           		// attaches the servo on pin 16 to the servo object (PB2 MOSI PWM) // Sparkfun Pro Micro

} 

//-------------------------------------
// Main execution loop
//
// loop here to read the throttle and start/stop switch
//
void loop() 
{ 
 
	#ifdef __DEBUGCODE
	Serial.println(potvalue);
	#endif //__DEBUGCODE

	///*
	//
	// read the stop button (already debounced)
	//
	if (readSwitch() == true) {
	  smokeOn();
	  return;
	}
	else {
	  smokeOff();
	  return;
	}
	delay(10); 	// delay between reads for stability
	//*/
	
  /*smokeOn();
	delay(2000); 	
  smokeOff();
	delay(10000); 	*/
	
} 
