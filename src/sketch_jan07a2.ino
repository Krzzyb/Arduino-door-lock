#include <LiquidCrystal_I2C.h>
#include <Keypad.h> 
#include <Wire.h>
#include <Servo.h>
#include <time.h>
#define czujnik 11
#define PIR 10
const byte ROWS = 4; 
const byte COLS = 3; 
const int servoPin = 2;
Servo servo;
int cyfra = 0; 
char secret[5] = {'1','2','3','4'};
char kod[5];
byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3}; 
time_t ostatniaAkcja;
 
char keys[ROWS][COLS] = { 
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
 
Keypad klawiatura = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); 
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.noBacklight(); 
  servo.attach(servoPin);
  servo.write(90);
  pinMode(czujnik, INPUT_PULLUP);
  pinMode(PIR, INPUT);
  ostatniaAkcja = time(NULL);
  }
  
void loop(){ 
 ReagujNaKlawisz();
 UstawEkran();
 PodswietlEkran();
}

void ReagujNaKlawisz()
{ 
  char klawisz = klawiatura.getKey();
  if( klawisz )
  {
    ostatniaAkcja = time(NULL);
    switch(klawisz)
    {
      case '#':
        cyfra = 0;
        break;
      default:
        kod[cyfra] = klawisz;
        cyfra = cyfra + 1;
        break;
    }
    lcd.clear();
  }
}

void UstawEkran()
{  
  lcd.setCursor(0,0);
  lcd.print("Wpisz kod:");
  lcd.setCursor(0,1);
  switch (cyfra)
  {
    case 0:
      break;
    case 1:
      lcd.print("*");
      break;
    case 2:
      lcd.print("**");
      break;
    case 3:
      lcd.print("***");
      break;
    case 4:
      SprawdzKod();
      break;
  }
}

void SprawdzKod()
{
  if(strcmp(kod, secret)==0)
    {
        lcd.clear();
        lcd.print("Prosze wejsc");
        servo.write(180);
        delay(2000);
        czujnikKrancowy();
        
        
    }else
    {
      lcd.clear();
      lcd.print("Kod niepoprawny");
      delay(1000);
      cyfra = 0;
      lcd.clear();
    }
}
void czujnikKrancowy(){
  if (digitalRead(czujnik) == LOW){
    servo.write(90);
    cyfra = 0;
    lcd.clear();    
  }
}

void PodswietlEkran()
{
  if(digitalRead(PIR) == HIGH){
    lcd.backlight();
  }
  else{
  
    lcd.noBacklight();
  } 
}
