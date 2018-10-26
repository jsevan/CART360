#include <dummy.h>
#include <SPI.h>
#include <WiFi.h>
#include <Twitter.h>
#include <ArduinoJson.h>

int fsrReading;
int fsrPin = 0

#define firebaseURL
#define authCode

char ssid[] = "VIDEOTRON3680";
char pass[] = "504w4ke334";

Twitter twitter("1055339040843665408-2IVSwDsmQf03yG5L9JbOYgZv4cH1ai");

// Message to post
char msg1[] = "Well aren’t you cold?  ";
char msg2[] = "Getting warmer…";
char msg3[] = "It’s warm!";

void setup()
{
  delay(1000);
  WiFi.begin(ssid, pass);
  delay(10000);
  Serial.begin(9600);


}

void loop()
{

  srReading = analogRead(fsrPin);

  Serial.print("Analog reading = ");
  Serial.print(fsrReading);     // the raw analog reading

  Serial.println("connecting ...");

  if (fsrReading < 100) {

    if (twitter.post(msg1)) {

      int status = twitter.wait(&Serial);
      if (status == 200) {
        Serial.println("OK.");
      } else {
        Serial.print("failed : code ");
        Serial.println(status);
      }
    } else {
      Serial.println("connection failed.");
    }
  }
} else if (fsrReading < 200) {
  if (twitter.post(msg2)) {

    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
}
} else if (fsrReading > 200) ||  {
  if (twitter.post(msg3)) {

    int status = twitter.wait(&Serial);
    if (status == 200) {
      Serial.println("OK.");
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
}

}
}




