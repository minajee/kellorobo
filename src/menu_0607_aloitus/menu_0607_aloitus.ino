#include <LiquidCrystal.h>

uint8_t time_h =12;
uint8_t time_min = 34;
uint8_t alarm_h = 98;
uint8_t alarm_min = 76;

const uint8_t rot_pin1 = 2;
const uint8_t rot_pin2 = 3;
const uint8_t rot_button = 1;
int rot_pos = 128;

volatile uint8_t screen = 0; // 0 = aika, 1 = heratys, 2 = demo
char temp1[16];
char temp2[16];
LiquidCrystal lcd(A5, A4, A0, A1, A2, A3); //RS, EN, D4, D5, D6, D7

void setup() {
  // put your setup code here, to run once:
  pinMode(rot_pin1, INPUT_PULLUP);
  pinMode(rot_pin2, INPUT_PULLUP);
  pinMode(rot_button, INPUT_PULLUP);
  
  lcd.begin(16, 2);                          // init display

  attachInterrupt(digitalPinToInterrupt(2), myEncoder, FALLING);  // encoder pin on interrupt 0
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
    rot_pos--;
  }
  else if (sum == 80) {
    rot_pos++;
  }
  //delay(5);
  interrupts();
}

void menu() {
  lcd.clear();
  sprintf(temp1, " Kello on %02d:%02d ", time_h, time_min);
  lcd.print(temp1);
  uint8_t prev_pos = rot_pos; 

  while(1)
  {
    if(rot_pos < prev_pos)
    {
      if(screen == 1) {
        sprintf(temp1, " Kello on %02d:%02d ", time_h, time_min);
        sprintf(temp2, "                ");
      }
      else if(screen == 2) {
        sprintf(temp1, "Heratys on %02d:%02d", alarm_h, alarm_min);
        sprintf(temp2, "*Onko ajastettu?*");
      }
      else if(screen == 0) {
        sprintf(temp1, "Paina ajaaksesi ");
        sprintf(temp2, "      demo      ");
      }
      lcd.setCursor(16, 0);
      lcd.print(temp1);
      lcd.setCursor(16, 1);
      lcd.print(temp2);
      for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(90);
      }
      if (screen == 0) {
        screen = 2;
      }
      else {
        screen--;
      }
      prev_pos = rot_pos;
    }
    else if (rot_pos > prev_pos)
    {
      if(screen == 2) {
        sprintf(temp1, " Kello on %02d:%02d ", time_h, time_min);
        sprintf(temp2, "                ");
      }
      else if(screen == 0) {
        sprintf(temp1, "Heratys on %02d:%02d", alarm_h, alarm_min);
        sprintf(temp2, "*Onko ajastettu?*");
      }
      else if(screen == 1) {
        sprintf(temp1, "Paina ajaaksesi ");
        sprintf(temp2, "      demo      ");
      }
      lcd.setCursor(-16, 0);
      lcd.print(temp1);
      lcd.setCursor(-16, 1);
      lcd.print(temp2);
      for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayRight();
        // wait a bit:
        delay(90);
      }     
      if (screen == 2) {
        screen = 0;
      }
      else {
        screen++;
      }
      prev_pos = rot_pos;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  menu();
  delay(50);
}
