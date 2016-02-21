
//include the neo pixel library
#include <Adafruit_NeoPixel.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"





/*NEOPIXELS*/

//the number of LEDs
static const int NUM_LEDS = 169;
static const int NUM_LEDS2 = 72;


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel (169, 6, NEO_GRB +NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel (72, 7, NEO_GRB +NEO_KHZ800);


int analogValueClap = 0;
int sensorClap = 0;
int ledRandom;
int R = 255;
int G,B;


/*PULSE SENSOR*/
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin
int pulsePin = 0;     // Pulse Sensor purple wire connected to analog pin 0
boolean firstPulse = false;


// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, must be seeded! 
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.




/*MOTOR*/

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);





void setup(){
  /*NEOPIXELS*/
  strip1.begin();
  strip1.show();
  
  strip2.begin();
  strip2.show();

  // declare the strip pin as an output:
  pinMode (6, OUTPUT);
  pinMode (7, OUTPUT);


  /*PULSE SENSOR*/
  Serial.begin(9600);             // we agree to talk fast!
  interruptSetup();  



/*MOTOR*/
  
   
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(10);  // 10 rpm 
 




}



void loop(){  

  
  
  /*PULSE SENSOR*/

  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
    
    firstPulse = true;
    strip1.setBrightness(10); //  fade LED
    delay(50);
    strip1.setBrightness(255); //  fade LED


 
    strip2.setBrightness(10); //  fade LED
    delay(50);
    strip2.setBrightness(255); //  fade LED

    fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
    QS = false;                      // reset the Quantified Self flag for next time    
    //Serial.println("beat");






  }

  ledFadeToBeat();

  strip1.show();   
  strip2.show();   
    
  
  if (firstPulse) {
  
  /*NEOPIXELS*/

  //  //read the sensor CLAP
  analogValueClap = analogRead (A2);
  sensorClap = map(analogValueClap, 0, 1005, 0, 255);   
  Serial.println(analogValueClap);
  
  
  if (sensorClap > 80){
     R = random(0,255);
     G = random(0,255);
     B = random(0,255);
    
  }
  
   for(int i=0;i<NUM_LEDS;i++){
    strip1.setPixelColor(i, strip1.Color(R,G,B)); 
  }
  
   for(int i=0;i<NUM_LEDS2;i++){
    strip2.setPixelColor(i, strip2.Color(R,G,B)); 
  }
  


 
    /*MOTOR*/


  //Serial.println("Motor Microstep steps");
  myMotor->step(1, FORWARD, MICROSTEP); 
  myMotor->release();
  

  }
}  



void ledFadeToBeat(){
  fadeRate -= 40;                         //  set LED fade value
  fadeRate = constrain(fadeRate,50,255);   //  keep LED fade value from going into negative numbers!
  strip1.setBrightness(fadeRate); //  fade LED
  strip2.setBrightness(fadeRate); //  fade LED

}
