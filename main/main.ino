#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(10, 12, 5, 4, 3, 2);

int relePins[] = {14, 15, 16, 17, 18, 19, 20, 21, 62, 63}; 
int ledPin = 13;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando...");

  lcd.begin(16, 2);
  lcd.print("                "); // Limpa linha
  lcd.print("Hello, Arduino!");


  for (int i = 0; i < 10; i++) {
    pinMode(relePins[i], OUTPUT);
  }
}

void loop() {
    // Pisca LED para indicar execução
  digitalWrite(ledPin, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  delay(250);

  for (int i = 0; i < 10; i++) {
  digitalWrite(relePins[i], HIGH); // Ativa pino [i]
  Serial.print("Ligando pino ");   // Retorna no terminal
  Serial.println(relePins[i]);     // Retorna o número do pino no terminal

  lcd.setCursor(0, 1);           // Posiciona o cursor
  lcd.print("                "); // Limpa linha
  lcd.print("Ligando pino ");    // Retorna no terminal
  lcd.print(int(i));             // Retorna o número do pino no terminal
  delay(1000);                   // Atrasa 1s

  digitalWrite(relePins[i], LOW);     // Desativa pino [i]
  Serial.print("Desligando pino ");   // Retorna no terminal
  Serial.print(relePins[i]);          // Retorna o número do pino no terminal

  lcd.setCursor(0, 1);              // Posiciona o cursor
  lcd.print("                ");    // Limpa linha
  lcd.print("Desligando pino ");    // Retorna no termin  
  lcd.print(int(i));                // Retorna o número do pino no terminal
  delay(1000);                      // Atrasa 1s
  }
}
