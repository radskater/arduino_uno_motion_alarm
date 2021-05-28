//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//motion sensor declerations

//the time we give the sensor to calibrate (10-60 secs according to the datasheet) 
int calibrationTime = 30; //the time when the sensor outputs a low impulse
long unsigned int lowIn;
//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped
long unsigned int pause = 5000; 
boolean lockLow = true; 
boolean takeLowTime; 
int pirPin = 4; //the digital pin connected to the PIR sensor's output 
int ledPin = 5;

//buzzer declerations
int buzzer = 3;



void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(1,1);
  lcd.print("Ywrobot Arduino!");
   lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
   lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");

  //motion sensor code
   
   Serial.begin(9600);
   pinMode(pirPin, INPUT);
   pinMode(ledPin, OUTPUT);
   digitalWrite(pirPin, LOW);
   //give the sensor some time to calibrate
   Serial.print("calibrating sensor ");
   for(int i = 0; i < calibrationTime; i++)
   {
     Serial.print(".");
     delay(1000);
   }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);
  
}


 //LOOP 
  void loop()
  {
    bool stop = false;
    
    if(digitalRead(pirPin) == HIGH)
    {
      digitalWrite(ledPin, HIGH);
      //the led visualizes the sensors output pin state
      if(lockLow)
      {
        //makes sure we wait for a transition to LOW before any further output is made:
        lockLow = false;
        Serial.println("---");
        Serial.print("motion detected at ");

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("motion detected!");

        //activate buzzer
        if (stop == false)
        {
           for(int i = 0; i < 3; i++)
           {
              tone(buzzer, 460);
              delay(500);
              noTone(buzzer);
             delay(500);
             stop = true;
          }
        }   
        noTone(buzzer);
        
        Serial.print(millis()/1000);
        Serial.println(" sec");
        delay(50);
      }
        takeLowTime = true;
     }
        if(digitalRead(pirPin) == LOW)
        {
          digitalWrite(ledPin, LOW);
          //the led visualizes the sensors output pin state
          if(takeLowTime)
          {
            lowIn = millis();
            //save the time of the transition from high to LOW
            takeLowTime = false;
            //make sure this is only done at the start of a LOW phase
          }
            //if the sensor is low for more than the given pause,
            //we assume that no more motion is going to happen 
            if(!lockLow && millis() - lowIn > pause)
            {
              //makes sure this block of code is only executed again after
              //a new motion sequence has been detected
              lockLow = true;
              Serial.print("motion ended at ");

              //print on lcd
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("motion ended.");
              
              //output
              Serial.print((millis() - pause)/1000);
              Serial.println(" sec");
              delay(50);
            }
        }
   }
     
   
  
