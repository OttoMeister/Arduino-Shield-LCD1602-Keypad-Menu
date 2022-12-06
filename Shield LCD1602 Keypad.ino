#include <LiquidCrystal.h>
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int lcd_key = 0;
int adc_key_in = 0;
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5
int read_LCD_buttons() {
  adc_key_in = analogRead(0); 
   //RIGHT=0,UP=133,DOWN=309,LEFT=481,SELECT=721,NONE=1023  
  if (adc_key_in > 1000) return btnNONE; 
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 250) return btnUP;
  if (adc_key_in < 450) return btnDOWN;
  if (adc_key_in < 650) return btnLEFT;
  if (adc_key_in < 850) return btnSELECT;
  return btnNONE;
}
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Booting...");


  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Hola mundo!");
}
void loop() {
  analogWrite(10, 50);  //analogWrite(10,255);
  lcd.setCursor(9, 1);
  lcd.print(millis());
  lcd.setCursor(12, 0);
  lcd.print(analogRead(0));
  lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd_key = read_LCD_buttons();
  if (lcd_key == btnRIGHT) lcd.print("RIGHT ");
  else if (lcd_key == btnLEFT) lcd.print("LEFT  ");
  else if (lcd_key == btnUP) lcd.print("UP    ");
  else if (lcd_key == btnDOWN) lcd.print("DOWN  ");
  else if (lcd_key == btnSELECT) lcd.print("SELECT");
  else if (lcd_key == btnNONE) lcd.print("NONE  ");
}