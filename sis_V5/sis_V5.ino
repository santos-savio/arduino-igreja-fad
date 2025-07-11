#include <LiquidCrystal.h>
#include "DHT.h"

#define DHTPIN A12     // what pin we're connected to

#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);
#define BAUD_RATE 9600
#define TERM_CHAR '\n'
#define BUF_LEN  30


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
int count;


int ContatoSecoState1 = 0;        // variable for reading the status
int ContatoSecoState2 = 0;        // variable for reading the status
int ContatoSecoState3 = 0;        // variable for reading the status
int ContatoSecoState4 = 0;        // variable for reading the status
int ContatoSecoState5 = 0;        // variable for reading the status
int ContatoSecoState6 = 0;        // variable for reading the status
int ContatoSecoState7 = 0;        // variable for reading the status
int ContatoSecoState8 = 0;        // variable for reading the status



const int rele1 =  22;      // Rele 1 PA0
const int rele2 =  23;      // Rele 2 PA1
const int rele3 =  24;      // Rele 3 PA2
const int rele4 =  25;      // Rele 4 PA3
const int rele5 =  26;      // Rele 5 PA4
const int rele6 =  27;      // Rele 6 PA5
const int rele7 =  28;      // Rele 7 PA6
const int rele8 =  29;      // Rele 8 PA7
const int rele9 =  42;      // Rele 8 PA7
const int rele0 =  43;      // Rele 8 PA7

const int contatoseco8 = 41;    // the number of the pushbutton pin
const int contatoseco7 = 40;    // the number of the pushbutton pin
const int contatoseco6 = 49;    // the number of the drybutton pin
const int contatoseco5 = 48;    // the number of the drybutton pin
const int contatoseco4 = 47;    // the number of the drybutton pin
const int contatoseco3 = 46;    // the number of the drybutton pin
const int contatoseco2 = 45;    // the number of the drybutton pin
const int contatoseco1 = 44;    // the number of the drybutton pin


const int Sirene = 33;    //Saida de 12 volts Até 1,5A
const int Discadora = 32;  //Saida de 12 volts até 1A

LiquidCrystal lcd(39, 38, 34, 35, 36, 37);

void setup() {
  lcd.begin(16, 2);

  // Print a message to the LCD.


  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(rele3, OUTPUT);
  pinMode(rele4, OUTPUT);
  pinMode(rele5, OUTPUT);
  pinMode(rele6, OUTPUT);
  pinMode(rele7, OUTPUT);
  pinMode(rele8, OUTPUT);
  pinMode(rele9, OUTPUT);
  pinMode(rele0, OUTPUT);
  pinMode(Discadora, OUTPUT);
  pinMode(Sirene, OUTPUT);


  pinMode(contatoseco8, INPUT);
  pinMode(contatoseco7, INPUT);
  pinMode(contatoseco6, INPUT);
  pinMode(contatoseco5, INPUT);
  pinMode(contatoseco4, INPUT);
  pinMode(contatoseco3, INPUT);
  pinMode(contatoseco2, INPUT);
  pinMode(contatoseco1, INPUT);


  lcd.print("Projeto Arduino");
  //enable serial datada print
  Serial.begin(9600);
  Serial2.begin(9600); //Porta bluetooth
  Serial3.begin(115200); //Porta Xbee
  // Todas as portas declaradas aqui aconseguem acionar reles com o seguinte comando _r1on e _r1off para o rele 2 _r2on e _r2off e assim por diante

  inputString.reserve(200);
}

void loop() {

  if (count == 2000) {
    float h = dht.readHumidity();
    // Read temperature as Celsius
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit
    float f = dht.readTemperature(true);
    lcd.setCursor(0, 1);
    lcd.print("RH:     ");
    lcd.setCursor(4, 1);
    lcd.print(h);
    lcd.setCursor(8, 1);
    lcd.print("Temp:      ");
    lcd.setCursor(13, 1);
    lcd.print(t);
    count = 0;
  }

  if (stringComplete) {
    
      if (inputString == "cseco\r\n") {
      Serial3.print("Estado dos contatos Secos :");  
      Serial3.print(digitalRead(contatoseco1));
      Serial3.print(digitalRead(contatoseco2));
      Serial3.print(digitalRead(contatoseco3));
      Serial3.print(digitalRead(contatoseco4));
      Serial3.print(digitalRead(contatoseco5));
      Serial3.print(digitalRead(contatoseco6));
      Serial3.print(digitalRead(contatoseco7));
      Serial3.println(digitalRead(contatoseco8));
      Serial3.println();
      
    }
    
     if (inputString == "temp\r\n") {
      Serial3.print("Temperatura: ");
      Serial3.println(dht.readTemperature());
      Serial3.print("Umidade: ");
      Serial3.println(dht.readHumidity());
      
    }
    
    if (inputString == "r0\r\n") {
      Serial3.println("ligando rele0");
      digitalWrite(rele0, !digitalRead(rele0));
    }

    if (inputString == "r1\r\n") {
      Serial3.println("ligando rele1");
      digitalWrite(rele1, !digitalRead(rele1));
    }

    if (inputString == "r2\r\n") {
      Serial3.println("ligando rele2");
      digitalWrite(rele2, !digitalRead(rele2));
    }
    


    if (inputString == "r3\r\n") {
      Serial3.println("ligando rele3");
      digitalWrite(rele3, !digitalRead(rele3));
    }

    
     if (inputString == "r4\r\n") {
      Serial3.println("ligando rele4");
      digitalWrite(rele4, !digitalRead(rele4));
    }

    if (inputString == "r5\r\n") {
      Serial3.println("ligando rele5");
      digitalWrite(rele5, !digitalRead(rele5));
    }
    
    if (inputString == "r6\r\n") {
      Serial3.println("ligando rele6");
      digitalWrite(rele6, !digitalRead(rele6));
    }

    
     if (inputString == "r7\r\n") {
      Serial3.println("ligando rele7");
      digitalWrite(rele7, !digitalRead(rele7));
    }

    if (inputString == "r8\r\n") {
      Serial3.println("ligando rele8");
      digitalWrite(rele8, !digitalRead(rele8));
    }
    
    if (inputString == "r9\r\n") {
      Serial3.println("ligando rele9");
      digitalWrite(rele9, !digitalRead(rele9));
    }
    
    
    Serial.print(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }




  if (Serial3.available()) {
    char inChar = Serial3.read();

    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;

    }
  }
 count++;
}



