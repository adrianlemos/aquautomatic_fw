/*
 This is a Exemple for a simple temperature controler

*/

// include the library code:
#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int lcd_bl = 8;
const int lm35 = A0;
const int triac_acionamento = 12;
const int led_iluminacao = 13;
const int led_status = 9;

//Constante dos Pinos de saida para os acionamentos
const int botao_01 = 11;        // Pino do botão 01

int ledStateBotao_01 = HIGH;    // Estado atual o pino de saida
int stateBotao_01_saida = 0;    // Estado da saida do botão 01
int stateBotao_01;              // Atual estado do reading botao_01 no pino de entrada
int lastStateBotao_01 = LOW;    // Estado anterior do readingbotao_01


// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can't be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 5;    // the debounce time; increase if the output flickers
unsigned long selectionValue = 0;

float temperature;

// PROTOTIPO DA FUNÇÃO

void checkbutton();




//FUNÇÃO DE CONFIGURAÇÃO

void setup() {
  // set up the LCD's number of columns and rows:
  Serial.begin(115200);
  Serial.println("\n Starting");
  
  lcd.begin(16, 2);
 
  pinMode(lcd_bl, OUTPUT);
  pinMode(triac_acionamento, OUTPUT);
  pinMode(led_iluminacao, OUTPUT);
  pinMode(led_status, OUTPUT);
  pinMode(botao_01, INPUT_PULLUP);

  
  digitalWrite(lcd_bl, HIGH);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  

  
  temperature = (float(analogRead(lm35))*4.83/(1023))*100;
  

  checkbutton();
  selectionValue++;
  delay(1);
 
  
  if (selectionValue >= 700){
      Serial.println(temperature);
        lcd.setCursor(0, 0);
  // print the number of seconds since reset:
   // Print a message to the LCD.
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  
      if(temperature <= 27.4){
        digitalWrite(triac_acionamento, HIGH);
        digitalWrite(led_status, HIGH);
      
      }else{
        digitalWrite(triac_acionamento, LOW);
        digitalWrite(led_status, LOW);
      };

  
      selectionValue = 0;
  
  
      }

  }


//FUNÇÕES

void checkbutton(){
  //Serial.println("entrou na rotina de checar Botao");
  
 // read the state of the switch into a local variable:
  int readingbotao_01 = digitalRead(botao_01);


  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (readingbotao_01 != lastStateBotao_01) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the readingbotao_01 is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

  
    // if the button state has changed:
    if (readingbotao_01 != stateBotao_01) {
      stateBotao_01 = readingbotao_01;
    

    while(stateBotao_01 == LOW){
      selectionValue++;
      delay(1);
      stateBotao_01 = digitalRead(botao_01);
    }

      
      // only toggle the LED if the new button state is HIGH
      if (stateBotao_01 == HIGH) {

        ledStateBotao_01 = !ledStateBotao_01;
      }
    }

  digitalWrite(led_iluminacao, ledStateBotao_01);

  }
  



  // save the readingbotao_01.  Next time through the loop,
  // it'll be the lastStateBotao_01:
  lastStateBotao_01 = readingbotao_01;

  
}
