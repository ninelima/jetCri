//-----------------------------------------------------------------------------
// PBS TJ20 Throttle controller
// (c) BVH Aviation 2015
//
// Author Dawie Botes 2015
// Date 2015-06-03
// Version 1.00 - Initial release
//

#include <Servo.h> 

// #define __DEBUGCODE

//-------------------------------------
// PBS TJ20A
//
// Period = 20ms
//
// Pulse Width:
// 0.9ms = Stop
// 1.0ms - Idle
// 2.0ms = 100% N2
// 

#define PWM_STOP 35
#define PWM_IDLE 45
#define PWM_FULL 141

#define BUT_PIN 2
#define POT_WIPER 2

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 

//-------------------------------------
// STOP, PWM = 0.9 ms
// 
void engineStop()
{
  myservo.write(PWM_STOP);  // STOP/OFF signal
}

//-------------------------------------
// IDLE, PWM = 1.0 ms
// 
void engineIdle()
{
  myservo.write(PWM_IDLE);  // IDLE signal
}

//
// N2 100%, FULL POWER, PWM = 2.0 ms
// 
void engineFullPower()
{
  myservo.write(PWM_FULL);  // N2 100%  signal
}

//-------------------------------------
// Start sequence
//
// 1)	Switch on power supply and set RC signals in the R/C SETTINGS menu. 
// 2)	Set STOP signal at the RC transmitter. The display will indicate STOP in Menu A.
// 3)	Set IDLE signal at the RC transmitter. The display will indicate IDLE in Menu A. 
// 4)	Set 100 % signal at the RC transmitter. The display will indicate ?H: -REL in Menu A
// If TH: +RUN is indicated, the whole actuating procedure must be repeated. 
// 5)	Within 6 seconds, you must set the RC transmitter signal to IDLE. 
//
void engineStart()
{
	engineStop();
	delay(2000);

	engineIdle();
	delay(2000);

	engineFullPower();  //100% N2
	delay(2000);
	
	engineIdle();
	delay(2000);
}

//-------------------------------------
// Set % power, 0 - 100 %
//
void engineSetPower(int precentPower)
{
	if (precentPower > 100) precentPower = 100;
	if (precentPower < 0) precentPower = 0;
	
  myservo.write(PWM_IDLE + ((PWM_FULL - PWM_IDLE) * precentPower / 100));
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
    delay(100);    						  // delay 100 ms to debounce switch
  	if (digitalRead(BUT_PIN) == LOW)     // the pin is still low after the delay 
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
	Serial.println("PBS-TJ20A Engine controller");
	#endif //__DEBUGCODE

	pinMode(BUT_PIN,  INPUT_PULLUP);  	// Button pin, internal 50K pullup enabled
	// pinMode(13, OUTPUT);           	// LED pin, cannot be used input without a pullodown beacause of LED
  
	//myservo.attach(11); 				// attaches the servo on pin 11 to the servo object (PB3 MOSI PWM) // Duemlanove
	myservo.attach(16);           		// attaches the servo on pin 16 to the servo object (PB2 MOSI PWM) // Sparkfun Pro Micro

} 

//-------------------------------------
// Main execution loop
//
// loop here to read the throttle and start/stop switch
//
void loop() 
{ 
	//
	// read the throttle potentiometer
	//
	int potvalue = analogRead(POT_WIPER);    
  
	#ifdef __DEBUGCODE
	Serial.println(potvalue);
	#endif //__DEBUGCODE

	//
	// read the stop button (already debounced)
	//
	if (readSwitch() == true) {
	  engineStop();
	  return;
	}
  
	//
	// calulate and set the power
	//
	// y = mx + c, with
	//   m = 100 / 256, c = -150  ... will yield 0 to 100% for 90 degree sweep.
	//
	int power =  (double) potvalue * 100.0 / 256.0  - 150.0;		 
	engineSetPower(power);
	
	delay(10); 	// delay between reads for stability
} 
