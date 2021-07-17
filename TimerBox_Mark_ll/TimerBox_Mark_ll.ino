#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define CLK 2
#define DT 3
#define SW 4
Servo servo1;
Servo servo2;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
boolean ifTimeSet=false;
boolean secondSet=false;
boolean minuteSet=false;
boolean hourSet=false;

boolean ifSelectStart=false;
boolean ifTimerFinish=false;
int hours=1;
int minutes=1;
int seconds=59;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

  void setup() {
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK);
  servo1.attach(9);
  servo2.attach(10);
   Serial.begin(9600);
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
   display.setTextSize(1.5);
   display.setTextColor(WHITE); 
  }

  void loop() {
    if(ifSelectStart){
     if(!ifTimerFinish){
     if(!ifTimeSet){
      setTime();
     }else{
      countdown();
     }
     }else{
      int btnState = digitalRead(SW);
      if (btnState == LOW) {
        if (millis() - lastButtonPress > 50) {
         resetTimer(); 
        }
       lastButtonPress = millis();
      }
     }
   }else{
      startPage();
   }
 }

  void countdown(){
    delay(1000); 
    if(seconds==0 && minutes==0 && hours==0){
      display.clearDisplay();
      display.setCursor(24,26);// – plot a pixel in the x,y coordinates     
      display.print("TIMES UP!");
      ifTimerFinish=true;
      display.display(); 
      rotateServos(90);
     // resetTimer();
    }else{
      seconds--;
      if(seconds<=0&& minutes>0){
        seconds=59;
        minutes--;
      }
      if(minutes==0&&hours>0){
        minutes=59;
        hours--;
      }
      display.clearDisplay();
      display.setCursor(24,26);// – plot a pixel in the x,y coordinates    
      if(hours<10){
        display.print(0);
        display.print(hours);
      }else{
       display.print(hours);
      }
      display.print(":"); 
      if(minutes<10){
        display.print(0);
        display.print(minutes);
      }else{
       display.print(minutes);
      }      display.print(":");
      if(seconds<10){
        display.print(0);
        display.print(seconds);
      }else{
       display.print(seconds);
      }      
      display.display(); 
    }
  }

  boolean setTime(){
    counter=0;
    boolean selectHours=false;
    boolean selectMinutes=false;
    boolean selectSeconds=false;
        while(!ifTimeSet){
      if(hourSet){
        if(minuteSet){
          if(secondSet){
            ifTimeSet=true;
            hourSet=false;
            minuteSet=false;
            secondSet=false;
          }else{
            setSeconds();
          }
        }else{
          setMinutes();
        }
      }else{
        setHours();
      }
    }
  }
  
  void setSeconds(){
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      if(counter>0){
        counter --;
      }
    } else{
        counter++;
    }
    display.clearDisplay();
    display.setCursor(24,26);
    display.print("Seconds:");
    display.print(counter); 
    display.display();
  }
  lastStateCLK = currentStateCLK;
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
    if (millis() - lastButtonPress > 50) {
      seconds=counter;
      secondSet= true;
      counter=0;
    }
    lastButtonPress = millis();
  }
  delay(1);
  }
  
  void setMinutes(){
   currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      if(counter>0){
        counter --;
      }
    } else{
        counter++;
    }
    display.clearDisplay();
    display.setCursor(24,26);
    display.print("Minutes:");
    display.print(counter); 
    display.display();
  }
  lastStateCLK = currentStateCLK;
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
    if (millis() - lastButtonPress > 50) {
      minutes=counter;
      minuteSet= true;
      counter=0;
      display.clearDisplay();
      display.setCursor(24,26);
      display.print("Seconds:");
      display.print(counter); 
      display.display();
    }
    lastButtonPress = millis();
  }
  delay(1);
  }
  
  void setHours(){
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      if(counter>0){
        counter --;
      }
    } else{
        counter++;
    }
    display.clearDisplay();
    display.setCursor(24,26);
    display.print("Hours:");
    display.print(counter); 
    display.display();
  }
  lastStateCLK = currentStateCLK;
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
    if (millis() - lastButtonPress > 50) {
      hours=counter;
      hourSet= true;
      counter=0;
      display.clearDisplay();
      display.setCursor(24,26);
      display.print("Minutes:");
      display.print(counter); 
      display.display();
      
    }
    lastButtonPress = millis();
  }
  delay(1);
}

void rotateServos(int deg) {//Unlocks both of the Servos
     servo1.write(deg);  
     servo2.write(deg);            
}

void startPage(){
    display.clearDisplay();
    display.setCursor(24,26);
    display.print("Click to start!");
    display.display();
    rotateServos(0);
        int btnState = digitalRead(SW);
    if (btnState == LOW) {
      if (millis() - lastButtonPress > 50) {
        ifSelectStart=true;
      display.clearDisplay();
      display.setCursor(24,26);
      display.print("Hours:");
      display.print(counter); 
      display.display();
      }
     lastButtonPress = millis();
    }
}

void resetTimer(){
  hours=0;
  minutes=0;
  seconds=0;
  counter=0;
  ifSelectStart=false;
  ifTimerFinish=false;
  ifTimeSet=false;
}
