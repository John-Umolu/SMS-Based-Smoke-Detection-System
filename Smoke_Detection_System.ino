#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

SoftwareSerial gsm(10, 11);
LiquidCrystal lcd(7,6,5,4,3,2);
//
//Please change to your mobile number
String gsmNumber = "+234xxxxxxxxxxxx";
//
int buz = 12;
int smsCount = 0;
float gasSensor = A0;
float gasValue;
float gasThreshold = 100.0;
int count = 0;
//
void setup() 
{
  Serial.begin(9600);
  gsm.begin(9600);
  lcd.begin(16,2);
  //
  pinMode(gasSensor, INPUT);
  pinMode(buz, OUTPUT);
  //
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please wait....!");
  delay(10000);
  gsm.println("AT+CMGDA=\"DEL ALL\"");     //Delete all Previous Messages
  delay(1000); 
  for(count = 0; count <= 100; count++){
    //
    gasValue = analogRead(gasSensor);
    delay(500);
    //
    if(gasValue >= gasThreshold){
      count = 1;
    }
    //
    Serial.println(gasValue);
    delay(500);
    lcd.setCursor(0,1);
    lcd.print("Warming Up: " + String(count) + "%");
    //
  }
  //
}

void loop() 
{
  //
  for(int x = 0; x<100; x++){
    gasValue = gasValue + analogRead(gasSensor);
  }
  gasValue = gasValue / 100.0;
  //
  Serial.println(String(gasValue));
  delay(1000);
  if(gasValue < gasThreshold){
    digitalWrite(buz, LOW);
    //
    lcd.setCursor(0,0);
    lcd.print("Normal Condition");
    lcd.setCursor(0,1);
    lcd.print(" No Smoke Found ");
    //
    smsCount = 0;
  }
  //
  if(gasValue >= gasThreshold){
    digitalWrite(buz, HIGH);
    //
    lcd.setCursor(0,0);
    lcd.print("Attention User!!");
    lcd.setCursor(0,1);
    lcd.print(" Smoke Detected ");
    //
    if(smsCount == 0){
      smsCount = 1;
      sendTextMessage();
    }
    //
  } 
}

void sendTextMessage(){
  //gsm routine
  gsm.println("AT+CMGF=1");
  delay(100);
  gsm.write("AT+CMGS=\"");    
  gsm.print(gsmNumber);    
  gsm.write("\"\r");   
  delay(100);
  gsm.println("Attention Please!!! Smoke Detected!");
  delay(100);
  gsm.println((char)26);                          //the ASCII code of the ctrl+z is 26
  delay(100);
  //Ends Sending SMS
  //Delete all Message 
  gsm.println("AT+CMGDA=\"DEL ALL\"");     //Delete all Previous Messages
  delay(100);
}
