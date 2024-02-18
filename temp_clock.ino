#include<Wire.h>
#include<string.h>
#include<LCD_I2C.h>
#include<DHT.h>
#define DHTPIN 8
#define DHTTYPE DHT11
#include <BfButton.h>
#include <BfButtonManager.h>

int rgb[3]={3,4,5};
String clr[3]={"blue","green","red"};
LCD_I2C lcd(0x27,16,2);
DHT dht(DHTPIN,DHTTYPE);
const int buzzer=11;
const int button_close=12;
const int button_open=2;
const int clk=7;
const int dt=6;
const int flame_sensor=13;
int aState;
int aLastState;
int butpin=A2;
BfButton btn(BfButton::STANDALONE_DIGITAL,butpin,true,LOW);

int time[3]={0,0,0};
int status=1;
int detect_open;
int detect_close;
int set_buttons[3]={10,9,13};
int new_detect_status[3]={0,0,0};

int rotary_encoder();
int rotary_encoder(){
  int a=0;
    aState=digitalRead(clk);
if(aState!=aLastState){
  if(digitalRead(dt)!=aState){

      a=1;

  }
  else{
      a=-1;  
  }
}
aLastState=aState;
return a;
}
void setup() {
  //setup code
  lcd.backlight();
  lcd.begin();
for(int i=0;i<3;i++){
  pinMode(rgb[i],OUTPUT);
}
pinMode(clk,INPUT);
pinMode(dt,INPUT);
pinMode(button_close,INPUT);
pinMode(button_open,INPUT);
aLastState=digitalRead(clk);
pinMode(buzzer,OUTPUT);
pinMode(flame_sensor,INPUT);
dht.begin();
Serial.begin(9600);

}

void loop() {
  detect_open = digitalRead(button_open);
  detect_close= digitalRead(button_close);
  int detect_flame=digitalRead(flame_sensor);
  if(detect_open==1){
    status=1;
  }
  else if(detect_close==1){
    status=0;
  }
  // put your main code here, to run repeatedly:
  if(status==1){
  delay(1000);
  time[2]=time[2]+1;
  if(time[2]==60 && time[1]!=60){
      lcd.clear();
      time[1]=time[1]+1;
      time[2]=0;    
  }
  else if(time[1]==59 && time[2]==59){
      lcd.clear();
      time[1]=0;
      time[2]=0;
      time[0]=time[0]+1;
  }
  lcd.setCursor(0,0);
  lcd.print("Time");
  lcd.setCursor(0,1);
  String SendDate="";
  for(int i=0;i<9;i=i+4){
    lcd.setCursor(i, 1);
    lcd.print(time[i/4]);
    if(time[i/4]<10){
      lcd.setCursor(i+1,1);
      lcd.print(" ");
    }
    lcd.setCursor(i+2,1);
    lcd.print(":");
    lcd.setCursor(10,1);
    lcd.print(" ");
    SendDate+=String(time[i/4])+":";
  }
  float h=dht.readHumidity();
  float t=dht.readTemperature();
  if(t>=22){
    tone(buzzer,588);
    analogWrite(rgb[2],225+t);
    analogWrite(rgb[0],0);
    analogWrite(rgb[1],0);    
  }
  else if(t<22 && t>=21){
    analogWrite(rgb[2],90-t/2);
    analogWrite(rgb[1],80+t/2);
    analogWrite(rgb[0],0);
  }
  else if(t<21 && t>=20){
    analogWrite(rgb[2],(t-20)*200);
    analogWrite(rgb[1],150);
    analogWrite(rgb[0],(21-t)*100);
    noTone(buzzer);
    digitalWrite(buzzer,LOW);
  }
  else if(t<20 && t>=19){
    analogWrite(rgb[0],(20.5-t)*170);
    analogWrite(rgb[1],255-(20.5-t)*170);
    analogWrite(rgb[2],0);
    noTone(buzzer);
    digitalWrite(buzzer,LOW);
  }
  else if(t<19){
    analogWrite(rgb[0],225+t); 
    analogWrite(rgb[1],0);
    analogWrite(rgb[2],0);
    noTone(buzzer);
    digitalWrite(buzzer,LOW);
  } 
  Serial.print(h);
  Serial.print(" ");
  Serial.print(t);
  Serial.print(" ");
  if(detect_flame==1){
    Serial.print(1);
    Serial.print(" ");
    tone(buzzer,200);
  }
  else if(detect_flame==0){
    Serial.print(0);
    Serial.print(" ");
    noTone(buzzer);
    digitalWrite(buzzer,LOW);
  }
  Serial.println(SendDate);
  // btn.read();
  }
  else if(status==0){
    
    noTone(buzzer);
    digitalWrite(buzzer,LOW);
    lcd.setCursor(0,0);

    lcd.print("Set Date");
    for(int i=0;i<3;i++){
      if(new_detect_status[i]==1 && digitalRead(set_buttons[i])==1){
          new_detect_status[i]=0;
      }
      else if(new_detect_status[i]==1 && digitalRead(set_buttons[i])==0){
        new_detect_status[i]=1;
      }
      else{
        new_detect_status[i]=digitalRead(set_buttons[i]);
      }
    
      if(time[i]<10 && time[i]>0){
        lcd.setCursor(1+i*2,1);
        lcd.print(" ");
      }
      if(time[i]<0){
        time[i]=59;
      }
      if(time[i]>59){
        time[i]=0;
      }
    }
    for(int i=0;i<3;i++){
      if(new_detect_status[i]==1){
        time[i]+=rotary_encoder();
      }
    }

    for(int i=0;i<9;i=i+4){
    lcd.setCursor(i, 1);
    lcd.print(time[i/4]);
    lcd.setCursor(i+2,1);
    lcd.print(":");
    lcd.setCursor(10,1);
    lcd.print(" ");
    }
  }
  
}

