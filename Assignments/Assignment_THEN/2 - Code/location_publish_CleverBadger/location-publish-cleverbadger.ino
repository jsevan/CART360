// This #include statement was automatically added by the Particle IDE.
// #include <neopixel.h>
// #define NUM_PIXELS 70
// #define PIN D2
// Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN);

#include <google-maps-device-locator.h>

GoogleMapsDeviceLocator locator;
#include <vector>
#include <string>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;

void setup() {
  Serial.begin(9600);
  
  // Scan for visible networks and publish to the cloud every 15 seconds
  // Pass the returned location to be handled by the locationCallback() method
  //locator.withEventName("smartBadger-location");
  locator.withSubscribe(locationCallback).withLocatePeriodic(15);
  
  Particle.subscribe("smartBadgerEvent", locationHandler);
}


void locationCallback(float lat, float lon, float accuracy) {
  // Handle the returned location data for the device. This method is passed three arguments:
  // - Latitude
  // - Longitude
  // - Accuracy of estimated location (in meters)
  String post = String(lat) +","+String(lon)+","+String(accuracy); 
  Serial.println(post);
  
  Particle.publish("cleverBadgerEvent", post, 60, PUBLIC); //PUBLIC OR PRIVATE OR DEVICE ID
}

void calculateGeographicDistance(vector<float> latlon, string d) {

   int earthRadius; 
   
   if(d == "km")
       earthRadius = 6371;
   else
       earthRadius = 3959;

   double diffLat = (latlon[2] - latlon[0]) * (M_PI / 180);
   double diffLon = (latlon[3] - latlon[1]) * (M_PI / 180);
   double a = sin(diffLat/2) * sin(diffLat/2) +
              cos(latlon[0] * (M_PI / 180)) * cos(latlon[2] * (M_PI / 180)) *
              sin(diffLon/2) * sin(diffLon/2);
   double c = 2 * atan2(sqrt(a), sqrt(1-a));
   double distance = earthRadius * c;

   Serial.print(distance); Serial.println("km");
}

void locationHandler(const char *event, const char *data) {
    
    Serial.println("Subscription Received: ");
    
  
    String str = String(data);
    
      vector<float> result;
    
    do
   {
       const char *begin = data;

       while(*data != ',' && *data)
           data++;

       result.push_back( strtof( string(begin, data).c_str(), 0 ));
   } while (0 != *data++);

   for(int i =0; i < result.size(); i++)
       Serial.println(result[i], 7);


 }
 
 
void loop() {
  locator.loop();

  
}