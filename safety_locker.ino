#include <SoftwareSerial.h>

#include <LiquidCrystal_I2C.h>



#include <Wire.h>

LiquidCrystal_I2C lcd(0x3f,20,4);
#include<EEPROM.h>
#include <Servo.h>
#include <Keypad.h>
char password[4];
char pass[4],pass1[4],pass2[4],pass3[4];
int i=0;
int timer=0;
int s=59;
int m=1;


int count=0;
int act=0;
char customKey=0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
  {'1','4','7','*'},
  {'2','5','8','0'},
  {'3','6','9','#'},
  {'A','B','C','D'}
};
byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
int buzzer = 11;
Servo myservo;
int mot_min = 90;   //min servo angle  (set yours)
int mot_max = 45; //Max servo angle   (set yours)




void setup(){
  myservo.attach(10); //attach the servo to pin D10
    pinMode(buzzer,OUTPUT); 
    lcd.init();
  lcd.backlight();
  lcd.clear();
  
  lcd.print(" SAFETY ");
  Serial.print(" SAFETY ");
 
  lcd.print(" LOCKER ");
  Serial.print(" LOCKER ");
  delay(2000);
  lcd.clear();
  lcd.print("Enter Passkey:");
  Serial.println("Enter Passkey:");
  lcd.setCursor(0,1);
  for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    pass[j]=EEPROM.read(j);
      //put the servo in the close position first
  myservo.write(mot_min);  

    Serial.begin(9600);
  
}


void loop(){
 customKey = customKeypad.getKey();
 
  if (customKey)
  {

    if(customKey=='C'&& act==0)
  {
   
     lcd.clear();
      lcd.print("Enter Passkey:");
      Serial.println("Enter Passkey:");
   
      lcd.setCursor(0,1);
     
      
      i=0;
    
    }

  else if(act==0){
    
    password[i++]=customKey;
    lcd.print("*");
    Serial.print(customKey);
  }
  }
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
      pass[j]=EEPROM.read(j);
     
    if(!(strncmp(password, pass,4)))
    {
    
      
      lcd.clear();
      lcd.print("Passkey Accepted");
      Serial.println("Passkey Accepted");
      count=0;
     myservo.write(mot_max); 
   SendMessage();
      
      lcd.clear();
     lcd.setCursor(0,0);
lcd.print("# TO CHANGE KEY ");
lcd.setCursor(1,1);
lcd.print("B TO CLOSE DOOR");
delay(1000);
     i=0;
  
       act=2;
   
      
    }
         
          
     else
    {
      
     digitalWrite(buzzer, HIGH);
     delay(500);
     digitalWrite(buzzer, LOW);
     delay(500);
     lcd.clear();
        lcd.print("Access Denied...");
        Serial.println("Access Denied...");
     digitalWrite(buzzer, HIGH);
     delay(500);
     digitalWrite(buzzer, LOW);
     delay(500);
     digitalWrite(buzzer, HIGH);
     delay(500);
     digitalWrite(buzzer, LOW);
        
      lcd.setCursor(0,1);
      lcd.print("Try Again");
      Serial.println("Try Again");
      count++;
      delay(2000);
      if(count<3){
      lcd.clear();
      lcd.print("Enter Passkey:");
      Serial.println("Enter Passkey:");
      lcd.setCursor(0,1);
      }
      i=0;
     
    }
  }
  if(count==3){
    lcd.clear();
    digitalWrite(buzzer, HIGH);
     delay(500);
     digitalWrite(buzzer, LOW);
     delay(500);
     digitalWrite(buzzer, HIGH);
     delay(500);
     digitalWrite(buzzer, LOW);
     call();
    lcd.print("Maximum Invalid");
    lcd.setCursor(0,1);
    lcd.print("Passkey Attempts");
    
    delay(2000);
    lcd.clear();

   count=0;
   //  timer=1;
    //lcd.clear();
      //lcd.print("Enter Passkey:");
      //Serial.println("Enter Passkey:");
      //lcd.setCursor(0,1);
      //i=0;

     
//-----------------------timer---------------------------------

//   lcd.clear();
  while(m>=0){
   
    lcd.setCursor(2,0); 
 lcd.print ("TRY AGAIN IN");
 s--;
 delay(1000);

  if(s<0)
 {
 m--;
 s=59;
 }
if(m>9)
 {
 lcd.setCursor(5,1);
 lcd.print(m);
 }
 else
 {
 lcd.setCursor(5,1);
 lcd.print("0"); 
 lcd.setCursor(6,1);
 lcd.print(m);
 lcd.setCursor(7,1);
 lcd.print(":");
 }
  
 if(s>9)
 {
 lcd.setCursor(8,1);
 lcd.print(s);
 }
 else
 {
 lcd.setCursor(8,1);
 lcd.print("0"); 
 lcd.setCursor(9,1);
 lcd.print(s);
 lcd.setCursor(10,1);
 lcd.print(" ");
 }


 
    }
  


    
  
 
  
    lcd.clear();
   lcd.print("Enter Passkey:");
     Serial.println("Enter Passkey:");
     lcd.setCursor(0,1);
      i=0;m=1;s=59;
       //count=0;
   //timer=0;
    
  }
   
//----------close door------------
  
   if (act == 2)
    {
    if(customKey == 'B' )
    {
      myservo.write(mot_min);
      act = 0;
     SendMessage1();
  
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Door closed");    
         delay(2000);
      lcd.clear();
      lcd.print("Enter Passkey:");
      Serial.println("Enter Passkey:");
        lcd.setCursor(0,1);
      i=0;
    
    }
     if(customKey=='#')
    change();
  }  
}



//-----------------------change password---------------------------------
void change()
{
   
  int j=0;
  lcd.clear();
  lcd.print("Current Passkey");
  Serial.println("Current Passkey");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=customKeypad.getKey();
    if(key)
    {
      pass1[j++]=key;
      lcd.print(key);
      Serial.print(key);
    
    }
    key=0;
  }
  delay(500);

  if((strncmp(pass1, pass, 4)))
  {
    lcd.clear();
    lcd.print("Wrong Passkey...");
    Serial.println("Wrong Passkey...");
    lcd.setCursor(0,1);
    lcd.print("Try Again");
    Serial.println("Try Again");
 
    delay(1000);
  }
  else
  {
    j=0;
    lcd.clear();
    lcd.print("New Passkey:");
    Serial.println("New Passkey:");
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=customKeypad.getKey();
      if(key)
      {
        pass2[j++]=key;
        lcd.print(key);
        Serial.print(key);
       
 
      }

     key=0;
    }
  
        j=0;
    lcd.clear();
    lcd.print("Confirm New Passkey:");
    Serial.println("Confirm New Passkey:");
    lcd.setCursor(0,1);
    while(j<4)
    {
      char key=customKeypad.getKey();
      if(key)
      {
        pass3[j++]=key;
        lcd.print(key);
        Serial.print(key);
       
 
      }

     key=0;
    }

  if((strncmp(pass2, pass3, 4)))
  {
    lcd.clear();
    lcd.print("New Passkey...");
    Serial.println("New Passkey...");
    lcd.setCursor(0,1);
    lcd.print("Does not match");
    Serial.println("Does not match");
    delay(2000);
     lcd.clear();
     
     lcd.print("Try Again");
    Serial.println("Try Again");
    delay(1000);
    count=0;
  }

    else{
      j=0;
      while(j<4){
        pass[j]=pass3[j];
            EEPROM.write(j,pass3[j]);
        j++;
        count=0;
        }

           digitalWrite(buzzer, HIGH);
     delay(200);
     digitalWrite(buzzer, LOW);
    lcd.print(" Done......");
    Serial.println(" Done......");
    
    SendMessage2();
  
    delay(1000);
    
  }

  

 
    
  }
  
  lcd.clear();
     lcd.clear();
     lcd.setCursor(0,0);
lcd.print("# TO CHANGE KEY ");
lcd.setCursor(1,1);
lcd.print("B TO CLOSE DOOR");
      
    
  customKey=0;
}

void SendMessage()
{
 Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.println("AT+CMGS=\"+918669036713\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("Locker has been opened");// The SMS text you want to send
  delay(100);
   Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
void SendMessage1()
{
 Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.println("AT+CMGS=\"+918669036713\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("Locker has been closed");// The SMS text you want to send
  delay(100);
   Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void SendMessage2()
{
 Serial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Serial.println("AT+CMGS=\"+918669036713\"\r"); // Replace x with mobile number
  delay(1000);
  Serial.println("Your password has been changed");// The SMS text you want to send
  delay(100);
   Serial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void call(){
  Serial.print("ATD08669036713;\n\r"); 
  }


