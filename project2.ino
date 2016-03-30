const int GSR=A2;

#include <Adafruit_NeoPixel.h>

#define PIN 10
#define LED_COUNT 10
#define TOP_DOWN 0
#define DOWN_TOP 1

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

int ledBrightness = 0;
const int pulsePin = A5;                 // Pulse Sensor purple wire connected to analog pin 0
//int blinkPin = 5;                // pin to blink led at each beat
int fadePin = 13;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded! 
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat". 
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse 


void setup(){
  Serial.begin(9600);             // we agree to talk fast!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  interruptSetup(); 
  
  leds.begin();  // Call this to start up the LED strip.
  clearLEDs();   // This function, defined below, turns all LEDs off...
  leds.show();   // ...but the LEDs don't actually update until you call this.

}

void loop(){
  
  int sensorValue=analogRead(GSR);
  Serial.print("sensorValue=");
  Serial.println(sensorValue);

  setLedColors(sensorValue);
  
  if (QS == true){
    ledBrightness = 255;
    QS = false;  
  } 

  setLedBrightness();
  delay(10);

}

 
void setLedColors(int gsrVal) 
{ 

//  uint32_t pixelColor = leds.Color(255, 255, 255);

  uint32_t pixelColor = 0;

  if (gsrVal >= 400) {
    pixelColor = leds.Color(0, 0, 255);
    
  } else if (gsrVal >= 300) {
    pixelColor = leds.Color(0, 255, 0);
    
  } else if (gsrVal >= 200) {
    pixelColor = leds.Color(255, 255, 0);
    
  } else if (gsrVal >= 100) {
    pixelColor = leds.Color(255, 69, 0);
    
  } else {
    pixelColor = leds.Color(255, 0, 0);
  }
  
  for (int i=0; i<LED_COUNT; i++)
  { 
    leds.setPixelColor(i, pixelColor);
  }
  leds.show();
}

void clearLEDs()
{
  for (int i=0; i<LED_COUNT; i++)
  {
    leds.setPixelColor(i, 0);
  }
}

void setLedBrightness(){
    ledBrightness -= 13;                         //  set LED fade value
    ledBrightness = constrain(ledBrightness,20,255);   //  keep LED fade value from going into negative numbers!  
   
    leds.setBrightness(ledBrightness);
}
