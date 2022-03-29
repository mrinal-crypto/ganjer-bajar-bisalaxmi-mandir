#include<DS3231.h>

#define signalPin 13

#define audioOut 5
#define lightOut 7

#define buz 9
#define error 8

void autoAudio();
void autoLight();
void Signal();
void buzzer();
void serial();  

unsigned long prevSerialMillis = 0;
unsigned long prevSignalMillis = 0;
const long serialInterval = 1000;
const long signalOnTime = 100;
const long signalOffTime = 1000;

int ledState = LOW;

DS3231  rtc(SDA, SCL);

int Hor;
int Min;
int Sec;
int Year;
int Month;
unsigned int workTime;

Time t;

/////////////////////////////////////////////////////

void autoAudio(){

  if(workTime>=270 && workTime<660 || workTime>=960 && workTime<1290){  //4.30=>11.00 16=>21.30
    digitalWrite(audioOut, HIGH);
  }
  else{
    digitalWrite(audioOut, LOW);
  }
}

////////////////////////////////////////////////////

void autoLight(){

  if(Month == 1 || Month == 2 || Month == 12){
    if(workTime>=1020 && workTime<1200){  //17=>20
      digitalWrite(lightOut, HIGH);
    } 
    else{
      digitalWrite(lightOut, LOW);
    }
  }
  else{
    if(workTime>=1080 && workTime<1260){ //18=>21
      digitalWrite(lightOut, HIGH);
    }
    else{
      digitalWrite(lightOut, LOW);
    }
  }
}

////////////////////////////////////////////////

void serial(){
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - prevSerialMillis >= serialInterval){
    
    prevSerialMillis = currentMillis;    
    Serial.print("Time: ");
    Serial.print(rtc.getTimeStr());
    Serial.print(" || Date: ");
    Serial.print(rtc.getDateStr(FORMAT_SHORT));
    Serial.print(" ");
    Serial.print(rtc.getDOWStr());
    Serial.print(" || temperature: ");
    Serial.print(rtc.getTemp());
    Serial.print(" C Month ");
    Serial.print(Month);
    Serial.print("\n");
  }


}

////////////////////////////////////////////////////

void Signal(){
  
  unsigned long currentMillis = millis();
  
  if((ledState == HIGH) && (currentMillis - prevSignalMillis >= signalOnTime)){
    
    ledState = LOW;
    prevSignalMillis = currentMillis;
    digitalWrite(signalPin, ledState);
  }
  
  else if ((ledState == LOW) && (currentMillis - prevSignalMillis >= signalOffTime)){
    
    ledState = HIGH;
    prevSignalMillis = currentMillis;
    digitalWrite(signalPin, ledState);
  }
  
}

///////////////////////////////////////////////////////

void buzzer(){
  
  for(int i=0; i<24; i++){
    
    if(i==Hor && Min==0 && Sec==0){
      digitalWrite(buz,HIGH);
      delay(1500);
      digitalWrite(buz,LOW);
    }
  }

}

///////////////////////////////////////////////////////////

void setup(){
  
    Serial.begin(9600);
    rtc.begin();
    pinMode(audioOut, OUTPUT);
    pinMode(lightOut, OUTPUT);
    pinMode(signalPin, OUTPUT);
    pinMode(error, OUTPUT);
    pinMode(buz, OUTPUT);

    delay(1000);
    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
    delay(50);
    digitalWrite(buz, HIGH);
    delay(100);
    digitalWrite(buz, LOW);
      
//rtc.setDOW(MONDAY);///set day by uncomment
//rtc.setTime(19,03,30);///set time
//rtc.setDate(17,1,2022); //set date

}

//////////////////////////////////////////////////////

void loop(){

  t = rtc.getTime();
  Hor= t.hour;
  Min= t.min;
  Month = t.mon;
  Year = t.year;
  workTime = (Hor*60)+Min;

  if(Year != 2000){
    autoAudio();
    autoLight();
    Signal();
    //buzzer();
    //serial();  
  }
  
  if(Year == 2000){
    digitalWrite(error, HIGH);
    digitalWrite(buz, HIGH);
    Serial.println("--- time not set ---");
    //serial();
    delay(150);
    digitalWrite(error, LOW);
    digitalWrite(buz, LOW);
    delay(150);
  }
}

////////////////////////////////////////////////
