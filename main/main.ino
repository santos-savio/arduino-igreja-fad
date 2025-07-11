#include <Wire.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <SPI.h>
#include "Ethernet.h"

// Configuração Ethernet (ajuste para sua rede)
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0x03, 0xD4 };
IPAddress ip(10, 31, 17, 14); // IP estático do Arduino
// IPAddress ip(10, 31, 19, 14); // IP estático do Arduino para a Vlan da igreja
EthernetServer server(80);          // Porta 80 (Para acessar via navegador)

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

boolean statusRele[] = {0,1,2,3,4,5,6,7,8,9};

const int Sirene = 33;    //Saida de 12 volts Até 1,5A
const int Discadora = 32;  //Saida de 12 volts até 1A

LiquidCrystal lcd(39, 38, 34, 35, 36, 37);
int relePins[] = {22, 23, 24, 25, 26, 27, 28, 29, 42, 43};
int ledPin = 13;

int info_speed = 8;    // Velocidade da exibição das informações do sensor

int ativaRele(int n) {
  Serial.print("Função ativaRele() acionada, rele n: ");
  Serial.println(n);
}

void leituraSensor() {
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
  if (tipoRetornoSensor == 0) {         // Exibe a humidade
    Serial.println(humidityStr);
    lcd.setCursor(0, 1);
    lcd.print(humidityStr);
    tipoRetornoSensor++;

  } else if (tipoRetornoSensor == 1) {  // Exibe a temperatura
    Serial.println(tempStr);
    lcd.setCursor(0, 1);
    lcd.print(tempStr);
    tipoRetornoSensor++;

  } else if (tipoRetornoSensor == 2) {  // Exibe a sensação térmica
    Serial.println(heatIndexStr);
    lcd.setCursor(0, 1);
    lcd.print(heatIndexStr);
    tipoRetornoSensor++;
  }

  else {
    lcd.setCursor(0, 1);              // Exibe @savio.job
    lcd.print("@savio.job      ");
    tipoRetornoSensor = 0;
    Serial.println("\n--------------------------------\n");
  }
}


void ativaTudo() {
    for (int i = 0; i < 10; i++) {
      digitalWrite(relePins[i], HIGH); // Ativa pino [i]
      Serial.print("Ligando relé ");   // Retorna no terminal
      Serial.println(i+1);     // Retorna o número do pino no terminal
    }
}

void desativaTudo() {
    for (int i = 0; i < 10; i++) {
      digitalWrite(relePins[i], LOW);     // Desativa pino [i]
      Serial.print("Desligando relé ");   // Retorna no terminal
      Serial.println(relePins[i]);          // Retorna o número do pino no terminal
      Serial.println("\n");
    }
}

void controleRele(int n) {
    Serial3.print("Estado do rele ");
    Serial3.println(n);
    digitalWrite(relePins[n], !digitalRead(relePins[n]));
}

void handleWebserver() {
  EthernetClient client = server.available(); // Verifica se há clientes
   if (client) {
    Serial.println("Novo cliente conectado!");
    boolean currentLineIsBlank = true;  // Flag para verificar o fim da requisição HTTP
    String request = "";               // Armazena a requisição do cliente

      while (client.available()) {
        char c = client.read();
        request += c;  // Acumula os caracteres da requisição

          // Fim da requisição HTTP (quando recebe uma linha em branco)
        if (c == '\n') {
          if (request.endsWith("\r\n\r\n")) {
            currentLineIsBlank = true;
            break;
          }
        }
      }

          // client.println("Location: /");  // redireciona para a raiz do servidor

          stringComplete = true;
          inputString = request;

          // Processa a requisição
          if (request.indexOf("GET /status") != -1) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            
              // Retorna o array com os status
            client.print("{\"dados\":[");
            for (int i = 0; i < 10; i++) {
            // statusRele[i] = digitalRead(relePins[i]);
            client.print(digitalRead(relePins[i]));
            Serial.print("Status rele ");
            Serial.print(i);
            Serial.print(digitalRead(relePins[i]));
            if (i < 9) client.print(",");
          }
          client.println("]}");
          client.stop();
          Serial.println("Função de status executada.");
          return; // Garante que ele não continue e mande o HTML depoi

          } else if (request.indexOf("POST /ativaTudo") != -1) {
            ativaTudo();
          } else if (request.indexOf("POST /desativaTudo") != -1) {
            desativaTudo();
          } else if (request.indexOf("POST /r0") != -1) {
            controleRele(0);
          } else if (request.indexOf("POST /r1") != -1) {
            controleRele(1);
          } else if (request.indexOf("POST /r2") != -1) {
            controleRele(2);
          } else if (request.indexOf("POST /r3") != -1) {
            controleRele(3);
          } else if (request.indexOf("POST /r4") != -1) {
            controleRele(4);
          } else if (request.indexOf("POST /r5") != -1) {
            controleRele(5);
          } else if (request.indexOf("POST /r6") != -1) {
            controleRele(6);
          } else if (request.indexOf("POST /r7") != -1) {
            controleRele(7);
          } else if (request.indexOf("POST /r8") != -1) {
            controleRele(8);
          } else if (request.indexOf("POST /r9") != -1) {
            controleRele(9);
          }

          // Resposta HTTP padrão
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();  // Fim dos cabeçalhos

          // Página HTML simples
          client.println("<!DOCTYPE html>");
          client.println("<html>");
          client.println("<head><title>Arduino Fad</title><meta charset=\"UTF-8\"></head>");
          client.println("<body>");
          client.println("<h1>Controle LUZ</h1>");

          // client.println("<p><a href=\"/\"><button>Atualizar</button></a></p>");
          client.println("<p><button data-comando='/ativaTudo' onclick='enviarComando(this)'>Ativar todos os reles</button></p>");
          client.println("<p><button data-comando='/desativaTudo' onclick='enviarComando(this)'>Desativar todos os reles</button></p>");
          // client.println("<p><button data-comando='/r0' onclick='enviarComando(this)'>Relé 1</button>"+ String(statusRele[0] ? " Ligado" : " Desligado" )+"</p>");

          client.println("<div><p><button data-comando='/r0' onclick='enviarComando(this)'>Relé 1</button><span id=\"status1\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r1' onclick='enviarComando(this)'>Relé 2</button><span id=\"status2\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r2' onclick='enviarComando(this)'>Relé 3</button><span id=\"status3\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r3' onclick='enviarComando(this)'>Relé 4</button><span id=\"status4\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r4' onclick='enviarComando(this)'>Relé 5</button><span id=\"status5\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r5' onclick='enviarComando(this)'>Relé 6</button><span id=\"status6\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r6' onclick='enviarComando(this)'>Relé 7</button><span id=\"status7\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r7' onclick='enviarComando(this)'>Relé 8</button><span id=\"status8\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r8' onclick='enviarComando(this)'>Relé 9</button><span id=\"status9\">  Carregando...</span></p></div>");
          client.println("<div><p><button data-comando='/r9' onclick='enviarComando(this)'>Relé 10</button><span id=\"status10\">  Carregando...</span></p></div>");

          client.println("<script>");
          client.println("function enviarComando(botao) {");
          client.println("  const url = botao.getAttribute('data-comando');");
          client.println("  fetch(url, { method: 'POST' })");
          client.println("    .then(response => {");
          client.println("      if (!response.ok) throw new Error('Erro no servidor');");
          client.println("      return response.text();");
          client.println("    })");
          client.println("    .then(data => {");
          client.println("      console.log(`Comando enviado com sucesso para ${url}`);");
          client.println("    })");
          client.println("    .catch(error => {");
          client.println("      alert('Erro ao executar o comando.');");
          client.println("    });");
          client.println("}");
          // client.println("</script>");

          // client.println("<script>");
          client.println("function atualizarStatus() {");
          client.println("  fetch('/status')");
          client.println("    .then(response => response.json())");
          client.println("    .then(data => {");
          client.println("      const estados = data.dados;");  // Exemplo: ["1", "1", "0"]
          client.println("      for (let i = 0; i < estados.length; i++) {");
          client.println("        const statusTexto = estados[i] == \"1\" ? \"  Ligado\" : \"  Desligado\";");
          client.println("        document.getElementById(`status${i+1}`).textContent = `${statusTexto}`;");
          client.println("      }");
          client.println("    })");
          client.println("    .catch(error => console.error(\"Erro ao atualizar status:\", error));");
          client.println("}");

          client.println("setInterval(atualizarStatus, 1000);  // Atualiza a cada 1 segundo");
          client.println("window.onload = atualizarStatus;");
          client.println("</script>");

          client.println("</body>");
          client.println("</html>");

    delay(1);       // Tempo para o cliente receber os dados
    // client.println("Location: /");  // redireciona para a raiz do servidor
    client.stop();  // Fecha a conexão
    Serial.println("Cliente desconectado.");
    Serial.println("Requisição recebida: " + request);
    loop();
  }
}


void setup() {
  Serial.begin(9600);      // Inicializa conexão serial
  Ethernet.begin(mac, ip); // Inicializa Ethernet
  server.begin();             // Inicializa servidor web

  String ipAtual = "10.31.17.14:23"; // Atualize com o ip real
  // String ipAtual = "10.31.19.14:23"; // Ip para a Vlan da igreja

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

  pinMode(ledPin, OUTPUT); // Inicializa Led 13
  inputString.reserve(200);
}

void loop() {
  handleWebserver();    // Inicializa o servidor web

  if (count == 10000) {
    count2++;
    count = 0;
  }

  if (count2 == info_speed) {
      for (int i = 0; i < 10; i++) {
        // String resultadoStatusRele = "Status rele " + String(i) + ": "+ String(statusRele[i] ? "Ligado" : "Desligado");
        // Serial.println(resultadoStatusRele);
      }
    leituraSensor();
    count2 = 0;
    }

  if (Serial.available()) {
    char inChar = Serial.read();
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }

  if (stringComplete) {

    if (inputString == "teste\r\n" || inputString == "teste") {
      Serial.println("Teste executado.\n");
      ativaRele(1);
    }

    if (inputString == "f1") {
      ativaTudo();
    }

    if (inputString == "f2") {
      desativaTudo();
    }
    
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
    
    if (inputString == "temp\r\n" || inputString == "temp") {
      Serial3.print("Temperatura: ");
      Serial3.println(dht.readTemperature());
      Serial3.print("Umidade: ");
      Serial3.println(dht.readHumidity());
      
    }
    
    if (inputString == "r0\r\n" || inputString == "r0") {
      Serial3.println("ligando rele0");
      digitalWrite(rele0, !digitalRead(rele0));
    }

    if (inputString == "r1\r\n" || inputString == "r1") {
      Serial.println("ligando rele1");
      digitalWrite(rele1, !digitalRead(rele1));
    }

    if (inputString == "r2\r\n" || inputString == "r2") {
      Serial3.println("ligando rele2");
      digitalWrite(rele2, !digitalRead(rele2));
    }
    

    if (inputString == "r3\r\n" || inputString == "r3") {
      Serial3.println("ligando rele3");
      digitalWrite(rele3, !digitalRead(rele3));
    }

    
    if (inputString == "r4\r\n" || inputString == "r4") {
      Serial3.println("ligando rele4");
      digitalWrite(rele4, !digitalRead(rele4));
    }

    if (inputString == "r5\r\n" || inputString == "r5") {
      Serial3.println("ligando rele5");
      digitalWrite(rele5, !digitalRead(rele5));
    }
    
    if (inputString == "r6\r\n" || inputString == "r6") {
      Serial3.println("ligando rele6");
      digitalWrite(rele6, !digitalRead(rele6));
    }

    
    if (inputString == "r7\r\n" || inputString == "r7") {
      Serial3.println("ligando rele7");
      digitalWrite(rele7, !digitalRead(rele7));
    }

    if (inputString == "r8\r\n" || inputString == "r8") {
      Serial3.println("ligando rele8");
      digitalWrite(rele8, !digitalRead(rele8));
    }
    
    if (inputString == "r9\r\n" || inputString == "r9") {
      Serial3.println("ligando rele9");
      digitalWrite(rele9, !digitalRead(rele9));
    }
    
    
    Serial.print(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

  count++; // Incrementa o contador para correta leitura do sensor
}