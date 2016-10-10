#include <Servo.h>

Servo servo1;  // create servo object to control a servo
Servo servo2;

int pos = 0;    // variable to store the servo position
char string[40];
char c;
int i = 0;
int input;
int s1_prev = 0, s2_prev = 0, s1, s2;

void setup(){
  Serial.begin(9600);
  servo1.attach(5);  // attaches the servo1 on pin 5 to the servo object
  servo2.attach(3); // And servo2 on pin 3
  s1=90;s2=180;
}

void loop(){
  
  while(Serial.available() > 0){
    c = Serial.read();
    if(c != ','){
     string[i] = c;
     string[i+1] = '\0';
     i++;  
    } else{
      sscanf(string, "%d:%d,", &s1, &s2);
      //s1 = (s1 + s1_prev < 0) ? 0 : (s1 + s1_prev);
      //s2 = (s2 + s2_prev < 0) ? 0 : (s2 + s2_prev);

      //s1_prev = s1;
      //s2_prev = s2;

      //Serial.println(s1);
      //Serial.println(s2);
      i=0;
    }    
  }

  servo1.write(s1);
  servo2.write(s2);
}
