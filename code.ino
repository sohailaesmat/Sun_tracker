
#include <Servo.h>
int  mode = 0;
int axe = 0;            
int buttonState1 = 0;    
int buttonState2  = 0;   
int prevButtonState1 = 0;
int prevButtonState2 = 0; 
 
int ldrtopr=  0;                                       
int ldrtopl = 1;                                    
int ldrbotr = 2;                                
int ldrbotl = 3;                                 
int topl = 0;
int topr = 0; 
int botl = 0;
int  botr = 0;

//Declare two servos
Servo servo_updown;
Servo servo_rightleft;

int  threshold_value=10;          

void setup()
{
  Serial.begin(9600);                               
  Serial.println("CLEARDATA");                      
  Serial.println("LABEL,t,voltage,current,power,Mode");  

  pinMode(12, INPUT);             
  pinMode(11, INPUT);             
  pinMode(A4,  INPUT);              
  
  servo_updown.attach(5);            
  servo_rightleft.attach(6);          
}

void loop()
{
//  pv_power();
char  Mode;
    float volt = analogRead(A5)*5.0/1023;
    float voltage = 2*volt;                
    float current = voltage/20;           
    float power  = voltage*current;
    Serial.print("DATA,TIME,"); 
    Serial.print(voltage);   
    Serial.print(",");
    Serial.print(current);    
    Serial.print(",");
    Serial.print(power);  
    Serial.print(",");
    
//    Serial.println(Mode);      
  buttonState1 = digitalRead(12);
  if (buttonState1 != prevButtonState1)  {
    if (buttonState1 == HIGH) {
      //Change mode and ligh up the correct  indicator  
      if (mode == 1) {
        mode = 0;
      } else {
        mode = 1;
      }
    }
  }
  prevButtonState1 = buttonState1;
  delay(50); 
  
  if (mode == 0) {
    Mode='M';
    Serial.println(Mode);       
    manualsolartracker();
  } else { // mode automatic
    Mode = 'A';
    Serial.println(Mode);      
    automaticsolartracker(); 
    } 
}

void  automaticsolartracker(){
  
     //capturing analog values of each LDR
     topr= analogRead(ldrtopr);        
     topl= analogRead(ldrtopl);         
     botr= analogRead(ldrbotr);        
     botl= analogRead(ldrbotl);         

    // calculating average
     int avgtop = (topr + topl) / 2;    
     int avgbot = (botr + botl) / 2;     
     int avgleft = (topl + botl) / 2;   
     int avgright  = (topr + botr) / 2;  
    //Get the different  
     int diffelev = avgtop - avgbot;      
     int diffazi = avgright - avgleft;    
    
    
     
      if (abs(diffazi) >= threshold_value){        
       if  (diffazi > 0) {
        if (servo_rightleft.read() < 180) {
          servo_rightleft.write((servo_updown.read()  + 2));
        }
      }
      if (diffazi <  0) {
        if (servo_rightleft.read()  > 0) {
          servo_rightleft.write((servo_updown.read() - 2));
        }
      }
    }
             
      

      if (abs(diffelev) >= threshold_value){    
       if (diffelev > 0) {
        if  (servo_updown.read() < 180) {
          servo_updown.write((servo_rightleft.read()  - 2));
        }
      }
      if (diffelev <  0) {
        if (servo_updown.read()  > 0) {
          servo_updown.write((servo_rightleft.read() + 2));
        }
      }
    }       
 }  

void manualsolartracker(){
  buttonState2  = digitalRead(13);
  if (buttonState2 != prevButtonState2) {
    if (buttonState2  == HIGH) {
      
      if  (axe == 1) {
        axe = 0;
      } else {
        axe = 1;
      }
    }
  }
  prevButtonState2 = buttonState2;
  delay(50); // Wait for 50  millisecond(s)
  if (axe == 0) {     
    servo_rightleft.write(map(analogRead(A4),  0, 1023, 0, 180));
  } else { 
    servo_updown.write(map(analogRead(A4),  0, 1023, 0, 180));
  } 
}
