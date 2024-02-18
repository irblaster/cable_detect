#include "Adafruit_VL53L0X.h"
#include <Adafruit_NeoPixel.h>


#define detected_cable 12
#define NEO_PIN 4
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 7
#define SHT_LOX2 6

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

int range1 = 0;
int range2 = 0;

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}


void set_pixel_strip_color(int r, int g, int b) {
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(r,g,b));

    pixels.show();   // Send the updated pixel colors to the hardware.
   
  }
}

void setup() {
  Serial.begin(115200);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }


  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  setID();

  //Serial.println("Adafruit VL53L0X test.");
  //if (!lox.begin()) {
  //  Serial.println(F("Failed to boot VL53L0X"));
//    while(1);
//  }
  // power
  //Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));

 



  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);

  pinMode (detected_cable, INPUT) ;
  digitalWrite (detected_cable, LOW) ;
 
 // start continuous ranging
 lox1.startRangeContinuous();
 lox2.startRangeContinuous();

}

void loop() {
  if (lox1.isRangeComplete()) {
    range1 = lox1.readRange();
  }
 if (lox2.isRangeComplete()) {
    range2 = lox2.readRange();
  }


  if (range1 < 100 and range2 > 300) {
      digitalWrite(LED_BUILTIN, HIGH);
      pinMode (detected_cable, OUTPUT) ;
      set_pixel_strip_color(0,255,0);
  } else {
      digitalWrite(LED_BUILTIN, LOW);
      pinMode (detected_cable, INPUT) ;
      set_pixel_strip_color(255,0,0);
  }
  Serial.print("Distance in mm: ");
  Serial.print(range1);
  Serial.print(" ");
  Serial.println(range2);
}