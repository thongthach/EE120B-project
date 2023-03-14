
// include the library code:
#include <LiquidCrystal.h>
#include <DHT.h>
#define DHTPIN A0     
#define DHTTYPE DHT11 

#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
int RECV_PIN = 6; //define input pin for the reciever on Arduino
IRrecv irrecv(RECV_PIN);
decode_results results;
DHT dht(DHTPIN, DHTTYPE);

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int i= 8;
int j= 9;
int led=13;
int counter;
int pwm=10;
int setTemp = 75;
int power;
int lastRan;

int temp;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
    
} task;

int delay_gcd;
const unsigned short tasksNum = 2;
task tasks[tasksNum];


 



enum SM1_States {  SM1_S0};
int SM1_Tick(int state1){
    switch(state1){ // State transitions
      case SM1_S0:
         state1=SM1_S0;
      break;
    }
    switch(state1){ // State Action
      case SM1_S0:

        
      if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX);
        if(results.value == 0xffa25d){
                
         if(power==1){
          
            power=0;
         }
         else{
          
             power=1;
          }  
        
        
        
      
   }
      else if(results.value == 0xff906f){
        setTemp++;
      }
     

      else if(results.value == 0xffe01f){
       setTemp--;
        
       }
   irrecv.resume();
  }
  break;

    }
    return state1;
}

    


enum SM2_States {  SM2_S0};
int SM2_Tick(int state2){
    switch(state2){

    case SM2_S0:
     if(power==1){
        
        int temp=dht.readTemperature(true); 
        
     
       if(temp>setTemp){
                digitalWrite(led,0);

                digitalWrite(i,LOW);
                digitalWrite(j,HIGH);
                
                
        }else if(temp<setTemp){
                 digitalWrite(led,1);
                digitalWrite(i, LOW);
                digitalWrite(j,LOW); 
                } 
         else if(temp==setTemp){
               digitalWrite(led,0);
               digitalWrite(i,LOW);
               digitalWrite(j,LOW);
          
         }
     }

        else if(power==0){
          digitalWrite(led,0);
          digitalWrite(i,0);
          digitalWrite(j,0);
            
          
       }

     
  
 
   break;

  }
    switch(state2){

      case SM2_S0:
          if(power==1){
              lcd.setCursor(0, 0);
              lcd.print("Temperature: ");
              lcd.print(int(temp));
             
              lcd.setCursor(0,1);
             lcd.print("Set Temp: ");
              lcd.print(setTemp);
          


       } 
        else if(power==0){
             lcd.clear();  
          
        }

        
       break;

      
}
 
    return state2;
}



     
    


  void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  pinMode(led,OUTPUT);
  pinMode(i,OUTPUT);
  pinMode(j,OUTPUT);
  lcd.clear();
  Serial.begin(9600);
   irrecv.enableIRIn();
  dht.begin();
   lastRan = 0;

   temp = dht.readTemperature(true);

   unsigned char i = 0;
  tasks[i].state = SM1_S0;
  tasks[i].period = 1;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM1_Tick;
  i++;


  tasks[i].state = SM2_S0;
  tasks[i].period = 10;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SM2_Tick;
 
}



   
  
  


void loop() {
    
 

  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
     if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
      
     }
   }
} 
  
  
 

  
 
