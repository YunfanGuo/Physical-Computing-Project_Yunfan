#include "HX711.h"
#include "WS2812B.h"

//---------------------------Attention---------------------------------------
//The pressure sensor will automatically initialise back before powering up, 
//so no pending weighing stuff, just wait 1 second!!!
//Pressure sensor SCK to 2, DT to 3 (to modify the pressure sensor pins move to the HX711.h library file)

//Ultrasonic pins
#define Echo  4
#define Trig  5

#define Ledpin   8      //WS2812B lamp with signal pin connected to 8
#define relay  9        //Relay signal pin is connected to pin 9 (default high triggered relay)

int brightness = 20;    //Brightness of the strip range 0-255, corresponding to 0-100% (too bright to consider whether the power supply can carry it)
int lednum = 150;        //Number of led lights in the strip
int pressure = 2000;    //If the pressure sensor is larger than 2kg, the light will turn on and run (range 0-20000g depending on the actual situation).
int distance = 100;     //If the distance measured is less than 100cm, turn on the LED strip .
//-------------------------------------------------------------------------------


int Weight = 0;
int ranging = 0;
// Function declarations
uint32_t rgbToColor(uint8_t r, uint8_t g, uint8_t b);
// Creating objects
WS2812B neoPixel_8;


uint32_t rgbToColor(uint8_t r, uint8_t g, uint8_t b)
{
  return (uint32_t)((((uint32_t)r << 16) | ((uint32_t)g << 8)) | (uint32_t)b);
}


float checkdistance() {   //Ultrasonic calculation functions
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  float distance = pulseIn(Echo, HIGH) / 58.00;
  delay(10);
  return distance;
}

void setup()
{
  Serial.begin(9600);
  neoPixel_8.begin(Ledpin, lednum);
  neoPixel_8.setBrightness(brightness);
  Init_Hx711();      //Initialise the IO settings for the HX711 module connection
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW); //Relay triggered high by default, so power up LOW Off
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  delay(100);
  Get_Maopi(); 
}

void loop()
{
  Weight = Get_Weight();  //Get the weight on the sensor
  ranging = checkdistance();  //Get ultrasonic distance measurement

  if (ranging <= distance) {    //Distance
    //RGB's colours are generated by random numbers
    neoPixel_8.setRangeColor(0, 0, rgbToColor(round((random(1, 255))), round((random(1, 255))), round((random(1, 255)))));
    delay(80);
    neoPixel_8.shift(1);  //1 unit of light
  } else {
    neoPixel_8.setRangeColor(0, lednum - 1, 0x000000);
  }

  if (Weight >= pressure) {    //Pressure
    digitalWrite(relay, HIGH);    //Open relay (high level HIGH)
  } else {
    digitalWrite(relay, LOW);     //Off relay (low level LOW)
  }
}
