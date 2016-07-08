#include <LiquidCrystal.h>

// Kellorobo

uint8_t time_h =0;
uint8_t time_min = 0;
uint8_t alarm_h =0;
uint8_t alarm_min = 0;

const uint8_t piezo_pin1 = 5;
const uint8_t piezo_pin2 = 4;

const uint8_t rot_pin1 = 2;
const uint8_t rot_pin2 = 3;
const uint8_t rot_button = 1;

const uint8_t motorA_ena = 11;
const uint8_t motorB_enb = 10;
const uint8_t motorA_in1 = 9;
const uint8_t motorA_in2 = 8;
const uint8_t motorB_in3 = 7;
const uint8_t motorB_in4 = 6;

int choice = 0;
volatile signed int location = 1;

LiquidCrystal lcd(A5, A4, A0, A1, A2, A3); //RS, EN, D4, D5, D6, D7

void setup()
{
  pinMode(piezo_pin1, OUTPUT);
  pinMode(piezo_pin2, OUTPUT);
  pinMode(rot_pin1, INPUT_PULLUP);
  pinMode(rot_pin2, INPUT_PULLUP);
  pinMode(rot_button, INPUT_PULLUP);
  pinMode(motorA_ena, OUTPUT);
  pinMode(motorB_enb, OUTPUT);
  pinMode(motorA_in1, OUTPUT);
  pinMode(motorB_in3, OUTPUT);
  pinMode(motorA_in2, OUTPUT);
  pinMode(motorB_in4, OUTPUT);
  
  lcd.begin(16, 2);                          // init display

  attachInterrupt(digitalPinToInterrupt(2), myEncoder, FALLING);  // encoder pin on interrupt 0 - pin 2

  digitalWrite(motorA_ena, LOW); // Motors are set to free run by default
  digitalWrite(motorB_enb, LOW);
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in3, LOW);
  digitalWrite(motorB_in4, LOW);

  Serial.begin(9600);
  Serial.println("Hello Computer");
}

int click() // Function to detect click on rotary encoder
{
  return digitalRead(rot_button);
}

void releaseClick() // Function to wait until user releases button
{
  while(1)
  {
    if (click() == 1);
    {
      delay(25);
      //Serial.println ("Click!");
      break;
    }
  }
}

void myEncoder()
{
  noInterrupts();
  int i;
  int sum = 0;
  int j;
  //Serial.println("jee");
  for (i=0; i< 10 ; i++){
    j = PIND & 0b00001100;
    //Serial.println(j);
    sum += j;
  }
  Serial.println(sum);
  if (sum == 0) {
    if (location == 1)
    {
      location = 4;
    }
    else
    {
      location--;
    }
  }
  else if (sum == 80) {
    if (location == 4)
    {
      location = 1;
    }
    else
    {
      location++;
    }
  }
  //delay(5);
  interrupts();
}

int menu()
{
  lcd.clear();
  lcd.print("  fwd <  > rev  ");
  lcd.setCursor(0, 1);
  lcd.print(" left <  > right");
  lcd.setCursor(6, 0);

  // location = 1;
  lcd.blink();
  //delay(3000);
  
  while(1)
  {
    if (click() == 0)
    {
//      releaseClick();
//      lcd.noBlink();
//      return location;
    }
    else
    {
      if (location == 1)
      {
        lcd.setCursor(6, 0);
      }
      
      else if (location == 2)
      {
        lcd.setCursor(9, 0);
      }
      
      else if (location == 4)
      {
        lcd.setCursor(6, 1);
      }
      
      else if (location == 3)
      {
        lcd.setCursor(9, 1);
      }
    }
  }
}

void drive(int direction)
{
  lcd.clear();
  lcd.print("CLICK!");
  
  digitalWrite(motorA_ena, HIGH);
  digitalWrite(motorB_enb, HIGH);
  
  if (direction == 1) // Forward
  {
    digitalWrite(motorA_in1, HIGH);
    digitalWrite(motorA_in2, LOW);
    digitalWrite(motorB_in3, HIGH);
    digitalWrite(motorB_in4, LOW);
    
    delay(2000);
  }
  
  else if (direction == 2) // Backward
  {
    digitalWrite(motorA_in1, LOW);
    digitalWrite(motorA_in2, HIGH);
    digitalWrite(motorB_in3, LOW);
    digitalWrite(motorB_in4, HIGH);
    
    delay(2000);
  }
  
  else if (direction == 4) // Left
  {
    digitalWrite(motorA_ena, LOW);
    
    digitalWrite(motorB_in3, HIGH);
    digitalWrite(motorB_in4, LOW);
    
    delay(700);
  }
  
  else if (direction == 3) // Right
  {
    digitalWrite(motorA_in1, HIGH);
    digitalWrite(motorA_in2, LOW);
    
    digitalWrite(motorB_enb, LOW);
    
    delay(700);
  }

  digitalWrite(motorA_ena, LOW);
  digitalWrite(motorB_enb, LOW);
  digitalWrite(motorA_in1, LOW);
  digitalWrite(motorA_in2, LOW);
  digitalWrite(motorB_in3, LOW);
  digitalWrite(motorB_in4, LOW);
}


void loop()
{
  // setup(); This is Arduino, this should be done anyway by default

  choice = menu();

  drive(choice);
}
