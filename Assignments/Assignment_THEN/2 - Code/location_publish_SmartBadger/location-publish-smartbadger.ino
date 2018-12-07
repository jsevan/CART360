// This #include statement was automatically added by the Particle IDE.
#include <neopixel.h>
#define NUM_PIXELS 70
#define PIN D2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN);

//FSR
int fsrAnalogPin = 0;
int fsrReading;

// This #include statement was automatically added by the Particle IDE.
#include <google-maps-device-locator.h>

GoogleMapsDeviceLocator locator;
#include <vector>
#include <string>
#include <cmath>
#define _USE_MATH_DEFINES

double myLat;double myLon;
double photonDistance;
using namespace std;

void setup() {
  Serial.begin(9600);
  //Use the neopixel library
  pixels.begin();
  pinMode(fsrAnalogPin, INPUT);
  // Scan for visible networks and publish to the cloud every 15 seconds
  // Pass the returned location to be handled by the locationCallback() method
  //locator.withEventName("smartBadger-location");
  locator.withSubscribe(locationCallback).withLocatePeriodic(15);
  Particle.subscribe("cleverBadgerEvent", locationHandler);
}


void locationCallback(float lat, float lon, float accuracy) {
  // Handle the returned location data for the device. This method is passed three arguments:
  // - Latitude
  // - Longitude
  // - Accuracy of estimated location (in meters)
  String post = String(lat) +","+String(lon)+","+String(accuracy); 
  Serial.println(post);
  
  
  myLat = lat; 
  myLon = lon;
  
  Particle.publish("smartBadgerEvent", post, 60, PUBLIC); //PUBLIC OR PRIVATE OR DEVICE ID
}

double calculateGeographicDistance(vector<float> latlon, string d) {
//THIS IS CALCULATING DISTANCE BETWEEN THE TWO POINTS USING LONGITUDE AND LATITUDE
   int earthRadius;
   if(d == "km")
       earthRadius = 6371;
   else
       earthRadius = 3959;


   double diffLat = (latlon[0] - myLat) * (M_PI / 180);
   double diffLon = (latlon[1] - myLon) * (M_PI / 180);
   double a = sin(diffLat/2) * sin(diffLat/2) +
              cos(latlon[0] * (M_PI / 180)) * cos(latlon[1] * (M_PI / 180)) *
              sin(diffLon/2) * sin(diffLon/2);
   double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double distance = earthRadius * c;

    
Serial.print(distance); Serial.println("km");
   return distance = earthRadius * c;
}

void locationHandler(const char *event, const char *data) {
    //THIS IS TOKENIZING - SO TURNING STRINGS INTO FLOATS AND SEPERATING EACH FLOAT
    Serial.println("Subscription Received: ");
    
    //Serial.println(data);
    String str = String(data);
    
      vector<float> result;
    
    do
   {
       const char *begin = data;

       while(*data != ',' && *data)
           data++;

       result.push_back( strtof( string(begin, data).c_str(), 0 ));
   } while (0 != *data++);

// Save the distance between the photons
    photonDistance = calculateGeographicDistance(result, "km"); 
}
//LEDS PROXIMITY
void activeProximityLEDS(double dist) {
    static int now = -1; // CLOSE, NEARER, NEAR, FAR, FURTHEST
    static int before = 0;

    //CONVERT TO METERS
    dist = dist * 1000;
    if(dist > 40) now = 0;
    else if((dist < 40) && (dist > 30)) now = 1;
    else if((dist < 30) && (dist > 20)) now = 2;
    else if((dist < 20) && (dist > 10)) now = 3;
    else if(dist < 10) now = 4;
    Serial.print("++++++++++");Serial.print(dist); Serial.println("++++++");
        switch(now) {
            case 0:
            // SET LEDS
                //FURTHEST
               fsrHandle();
                break;
            case 1:
                //FAR
                breatheGreen();
                break;
            case 2:
                //NEAR
                breatheRed();
                break;
            case 3:
                //NEARER
                breatheBlue();
                break;
            case 4:
                //CLOSE
                breatheWhite();
                break;
        }
        
    before = now;
}

//Set colors to each proximity
void breatheRed() {
    for(int i = 0; i < 255; i++) {
        setColors(i, 0, 0);
    }
}

void breatheGreen() {
    for(int i = 0; i < 255; i++) {
        setColors(0, i, 0);
    }
}

void breatheBlue() {
    for(int i = 0; i < 255; i++) {
        setColors(0, 0, i);
    }
}

void breatheWhite() {
    for (int i = 0; i < 255; i++) {
        setColors(i, i, i);
    }
}

void setColors(int r, int g, int b) {
    for(int i = 0; i < 71; i++){
        pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
}

//FSR functions
void fsrHandle() {
    int fsrReading = analogRead(fsrAnalogPin);
    Serial.println(fsrReading);
    
    //If it's not pressed, lights are turned off
    if (fsrReading < 50) {
        for (int i = 73; i >= 0; i--) {
            pixels.setPixelColor(i, pixels.Color(0, 0, 0));
            pixels.show();
            delay(10);
        }
    //If it's lightly pressed, lights are in a certain color    
    } else if (fsrReading > 50 && fsrReading < 350) {
        //light up the light
        //map the value received to a number between 0 and 255
        int ledNum = map(fsrReading, 0, 1023, 0, 255);
        for (int i = 0; i <ledNum; i++) {
            //Lights are turned in Salmon color
            pixels.setPixelColor(i, pixels.Color(250, 128, 114));
            pixels.show();
            delay(10);
        }
    //If it's medium pressed, lights are in a certain color    
    } else if (fsrReading > 350 && fsrReading < 650) {
        int ledNum = map(fsrReading, 0, 1023, 0, 255);
        for (int i = 0; i <ledNum; i++) {
            //Lights are turned in Pink color
            pixels.setPixelColor(i, pixels.Color(50, 0, 50));
            pixels.show();
            delay(10);
        }
    //If it's fully pressed, lights are in a certain color    
    } else if (fsrReading > 650 && fsrReading < 1023) {
        int ledNum = map(fsrReading, 0, 1023, 0, 255);
        for (int i = 0; i <ledNum; i++) {
            //Lights are turned in Lime color
            pixels.setPixelColor(i, pixels.Color(50, 205, 50));
            pixels.show();
            delay(10);
        }
    }
}

void loop() {
  locator.loop();
activeProximityLEDS(photonDistance);

}