//Smart trash bin
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

// WiFi Name and pass
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASS";


Servo Mservo;
int pos = 0;
bool condition; //Position of the servo right now (1 = open, 0 = closed)

byte servoPin = D4;

byte piezoPin = A0;
int thresholdPiezo = 10;
int value = 0;
 
//Ultra 1
byte echoPin = D1;
byte trigPin = D2;
long duration; // variable for the duration of sound wave travel
long realdistance;
float distance; // variable for the distance measurement
int thresholdUltra = 5;

//Ultra 2
byte echoPin2 = D7;
byte trigPin2 = D8;
long duration2; // variable for the duration of sound wave travel
long realdistance2;
float distance2; // variable for the distance measurement

int trashheight = 22;

float trash = 0;  //Data send

int piezoValue = 0;
int ultraValue = 0;

void setup() {
  Serial.println("Open");
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  Mservo.attach(servoPin, 500, 2400);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  Mservo.write(93);
  Serial.println("Connected");
}

void loop() {
  Blynk.run();

  piezoValue = piezo();
  ultraValue = ultra();
  
  //OPEN
  if((piezoValue == 1 || ultraValue == 1) && (condition == 0)){
    Serial.println("Open!!!!");
    for(pos = 93; pos < 180; pos += 4)  
    {
      Mservo.write(pos);
      delay(10);                 
    }
    condition = 1;  //Curently Open 
    delay(4000);
  }

  //CLOSE
  if((ultraValue != 1) && (condition == 1)){
    Serial.println("Close!!!!");
    for(pos = 180; pos >= 93; pos -= 4) 
      {
        Mservo.write(pos);
        delay(10);                 
      } 
    condition = 0; //Closed
    
    ultra2();  
  }

  delay(10);
}

bool piezo(){
  //Piezoelectric
  bool action = 0;
  value = analogRead(piezoPin);
  if (value >= thresholdPiezo) {
    action = 1;
    Serial.println("Pencet "+ (String)value);
  }
  else {
    action = 0;
  }
  return action;
}

//for detecting hand to open
bool ultra(){
  //Ultrasound
  bool action = 0;
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  if (distance > 0){
    realdistance = distance;
  }

  if ((realdistance <= thresholdUltra)){
    Serial.println("Dekat "+ (String)realdistance);
    action = 1;
  }
  else{
    action = 0;
  }

  return action;
}

//Ultrasound for detecting trash bin volume
void ultra2(){
  //Ultrasound
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  // Calculating the distance
  distance2 = duration2 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  if (distance2 > 0){
    if (distance2 > trashheight){
      realdistance2 = trashheight;
    }
    else{
       realdistance2 = distance2;
    }
  }
  Serial.println(realdistance2);
  trash = map(realdistance2, 3, trashheight, 100, 0);

  //send data to blynk
  Blynk.virtualWrite(V0, trash); 
  Serial.println(trash);
}
