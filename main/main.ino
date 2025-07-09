#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <SPI.h>
#include "Ethernet.h"

// Configuração Ethernet (ajuste para sua rede)
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x03, 0xD4 };
IPAddress ip(10, 31, 17, 14); // IP estático do Arduino
EthernetServer server(23);       // Porta 23 (Telnet, padrão para Putty)

#define DHTPIN A12
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
#define BAUD_RATE 9600  // Velocidade de resposta do terminal
#define TERM_CHAR '\n'  // Caratere para o fim de uma mensagem
#define BUF_LEN  30     // Tamanho do buffer

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
// Contadores para atraso no retorno da informação do sensor
int count = 0;
int count2 = 0;
int tipoRetornoSensor = 0;

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
const int rele9 =  42;      // Rele 9 PL7
const int rele0 =  43;      // Rele 10 PL6

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
int relePins[] = {22, 23, 24, 25, 26, 27, 28, 29, 42, 43};
int ledPin = 13;

int info_speed = 8;    // Velocidade da exibição das informações do sensor

void setup() {
  Serial.begin(9600);      // Inicializa conexão serial
  Ethernet.begin(mac, ip); // Inicializa Ethernet
  server.begin();

  String ipAtual = "10.31.17.13"; // Atualize conforme necessário

  Serial.println("\nIp atual: " + ipAtual);
  Serial.println("@savio.job \n");

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(ipAtual);
  lcd.setCursor(0, 1);
  lcd.print("@savio.job");
  // lcd.print("                "); // Limpa linha

  dht.begin(); // Inicializa o sensor DHT11

  // Define os pinos dos relés como saída
  for (int i = 0; i < 10; i++) {
    pinMode(relePins[i], OUTPUT);
  }

  pinMode(Discadora, OUTPUT); 
  pinMode(Sirene, OUTPUT);

  // Define os pinos dos contatos secos para leitura
  pinMode(contatoseco8, INPUT);
  pinMode(contatoseco7, INPUT);
  pinMode(contatoseco6, INPUT);
  pinMode(contatoseco5, INPUT);
  pinMode(contatoseco4, INPUT);
  pinMode(contatoseco3, INPUT);
  pinMode(contatoseco2, INPUT);
  pinMode(contatoseco1, INPUT);

  // Inicia com os relés desligados
    for (int i = 0; i < 10; i++) {
      digitalWrite(relePins[i], LOW);
    }

  pinMode(ledPin, OUTPUT); // Inicializa Led 13
}
void loop() {

  if (count == 30000) {
    // Serial.println("Interação 30k");
    count2++;
  }

  // if (count == 250000) {
    if (count2 == info_speed) {
    // Lê o sensor e exibe as informações somente a cada 2000 iterações do loop, sem travar a execução
    int h = dht.readHumidity();
    int t = dht.readTemperature();
    int f = dht.readTemperature(true);

      // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    int hic = dht.computeHeatIndex(t, h, false);

    // Criando as strings antes de exibi-las
    String humidityStr = "Humidade: " + String(h) + "%      ";
    String tempStr = "Temperat.: " + String(t) + "C"         ;
    String heatIndexStr = "Sen. term.: " + String(hic) + "C       ";

    // Exibindo as strings completas
    if (tipoRetornoSensor == 0) {
      Serial.println(humidityStr);
      lcd.setCursor(0, 1);
      lcd.print(humidityStr);
      tipoRetornoSensor++;

    } else if (tipoRetornoSensor == 1) {
      Serial.println(tempStr);
      lcd.setCursor(0, 1);
      lcd.print(tempStr);
      tipoRetornoSensor++;

    } else if (tipoRetornoSensor == 2) {
      Serial.println(heatIndexStr);
      lcd.setCursor(0, 1);
      lcd.print(heatIndexStr);
      tipoRetornoSensor++;
    }

    else {
      lcd.setCursor(0, 1);
      lcd.print("@savio.job      ");
      tipoRetornoSensor = 0;
      Serial.println("\n --------------------------------\n");
    }

    count = 0;
    count2 = 0;
  }



  // for (int i = 0; i < 10; i++) {
  //   // digitalWrite(relePins[i], HIGH); // Ativa pino [i]
  //   digitalWrite(ledPin, HIGH);      // Ativa led 13
  //   Serial.print("Ligando pino ");   // Retorna no terminal
  //   Serial.println(relePins[i]);     // Retorna o número do pino no terminal
  //   delay(3000);
  // }
  // delay(2000);
  // for (int i = 0; i < 10; i++) {
  //   digitalWrite(relePins[i], LOW);     // Desativa pino [i]
  //   digitalWrite(ledPin, LOW);          // Desativa led 13
  //   Serial.print("Desligando pino ");   // Retorna no terminal
  //   Serial.println(relePins[i]);          // Retorna o número do pino no terminal
  //   Serial.println("\n");
  //   delay(2500);
  // }
  count++; // Incrementa o contador para correta leitura do sensor
}
