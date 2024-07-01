#include<SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5,6, 7);
SoftwareSerial SIM800L(11,10); 

#define NOTE_C4 262 
#define NOTE_D4 294 
#define NOTE_E4 330 
#define NOTE_F4 349 
#define NOTE_G4 392 
#define NOTE_A4 440 
#define NOTE_B4 494 
#define NOTE_C5 523 
#define PHONE_NUMBER "0762728106" // Phone number to send SMS to

int temp;
int T_Sensor = A1;
int M_Sensor = A0;
int W_led = 8;
int P_led = 13;
int Speaker = 9;
int val;
int cel;  
bool above35Notified = false; // Flag to track whether a message has been sent for above 35
bool below35Notified = false; // Flag to track whether a message has been sent for below 35

void setup()
 {
  SIM800L.begin(9600);

  Serial.begin(9600);
  lcd.begin(16, 2);
    lcd.clear(); 
    pinMode(13,OUTPUT);
    pinMode(8,INPUT);
    pinMode(9,OUTPUT);
    
    val = analogRead(T_Sensor); //Read Temperature sensor value 
    int mv = ( val/1024.0)*5000; 
    cel = mv/10;
    
  
    
    lcd.setCursor(0,0);
    lcd.print("Smart Irrigation");
    lcd.setCursor(0,1);
    lcd.print("System");
    delay(1000);
    lcd.clear(); 
   
     lcd.clear(); 
    lcd.setCursor(0,0);
    lcd.print("Project By:");
    lcd.setCursor(0,1);
    lcd.print("amani");
    delay(1000);
}

void loop()
{
  
  lcd.clear();
  int Moisture = analogRead(M_Sensor); //Read Moisture Sensor Value 

  lcd.setCursor(0,0);
  lcd.print("TEMP:");
  lcd.setCursor(5,0);
  lcd.print(cel);
  lcd.setCursor(7,0);
  lcd.print("*C");
 
 if (Moisture> 700&& !above35Notified)   // for dry soil
  { 
        lcd.setCursor(11,0);
        lcd.print("DRY");
        lcd.setCursor(11,1);
        lcd.print("SOIL");
       if (digitalRead(W_led)==1) //test the availability of water in storage
       {
         digitalWrite(13, HIGH);
          sendSMS(PHONE_NUMBER, "Low Soil Moisture detected. Motor turned ON"); // Send SMS
          above35Notified = true; // Set flag to true to indicate message has been sent
          below35Notified = false; // Reset below 35 flag
         lcd.setCursor(0,1);
         lcd.print("PUMP:ON");
               }
       else
       {
         digitalWrite(13, LOW);
         sendSMS(PHONE_NUMBER, "Soil Moisture is Normal. Motor turned OFF"); // Send SMS
    below35Notified = true; // Set flag to true to indicate message has been sent
    above35Notified = false; // Reset above 35 flag
         lcd.setCursor(0,1);
         lcd.print("PUMP:OFF");
         
           
           tone(Speaker, NOTE_C4, 500); 
           delay(500); 
           tone(Speaker, NOTE_D4, 500);
           delay(500); 
           tone(Speaker, NOTE_E4, 500); 
           delay(500); 
           tone(Speaker, NOTE_F4, 500); 
           delay(500); 
           tone(Speaker, NOTE_G4, 500); 
           delay(500);
       }
    }
 
     if (Moisture>= 300 && Moisture<=700) //for Moist Soil
    { 
      lcd.setCursor(11,0);
     lcd.print("MOIST");
     lcd.setCursor(11,1);
     lcd.print("SOIL");
     digitalWrite(13,LOW);
     lcd.setCursor(0,1);
     lcd.print("PUMP:OFF");    
  }
 
  if (Moisture < 300)  // For Soggy soil
  { 
    lcd.setCursor(11,0);
     lcd.print("SOGGY");
     lcd.setCursor(11,1);
     lcd.print("SOIL");
     digitalWrite(13,LOW);
     lcd.setCursor(0,1);
     lcd.print("PUMP:OFF");
  }
 delay(1000);    
}


 void sendSMS(String number, String message) {
   SIM800L.println("AT+CMGF=1"); // Set the SMS mode to text
  delay(100);
  SIM800L.print("AT+CMGS=\"");
   SIM800L.print(number);
   SIM800L.println("\"");
  delay(100);
   SIM800L.println(message);
  delay(100);
  SIM800L.println((char)26); // End AT command with Ctrl+Z
  delay(100);
   SIM800L.println();
  delay(5000); // Wait for the SMS to be sent
}
