#include <Arduino.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h> 
#include <stdio.h>
#include <string.h>
#include "SoftwareSerial.h"
#include <cstring>

//*********RECEPCION************//
//U0_RXD ->  SIM808
//U2_RXD ->  WATIMETRO
//U3_RXD ->  Debug

//*********TRANSMISIÓN************//
//U0_TXD -> SIM808
//U2_TXD  -> WATIMETRO
//U3_TXD ->  Debug

//*************************
//      PINOUT
//*************************
//---TX2-RX2 (Watimetro)
#define RXp2 16
#define TXp2 17
//---TX3-RX3 (DEBUG)
#define RX3 (13)
#define TX3 (12)
//--LED ESP32
int LED = 2; 

//*************************
//      VARIABLES GLOBALES
//*************************
SoftwareSerial U3_Debug;
uint16_t corriente;
uint16_t voltaje;
uint16_t potencia;
uint16_t energia;

//#define MAX485_DE 12
#define MAX485_DE 32
#define led_esp32 2

//*************************
//       JSON
//*************************
StaticJsonDocument<512> doc;
String json;

//VARIABLES A REGISTRAR
int16_t corriente;
int16_t potencia;
int16_t voltaje;
int16_t energia; 

String servidor = "http://869c-38-25-16-250.sa.ngrok.io/api/lecturas/crearPorSerie/IM001";

//***********************
//   GSM
void comandosAT()
{
  Serial2.println("AT+HTTPINIT");
  delay(50);
  Serial2.println("AT+HTTPPARA=\"URL\",\""+servidor+"\""); //Server address
  delay(25);
  Serial2.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(25);
  Serial2.println("AT+HTTPDATA=" + String(json.length())  + ",1000");
  delay(50);
  Serial2.println(json);
  delay(300);
  json="";
  Serial2.println("AT+HTTPACTION=1");
  delay(25);
  Serial2.println("AT+HTTPTERM");
  delay(50);
  while(Serial2.available()!=0){/* If data is available on serial port */
    //Serial.write(char (Serial.read()));
    String cadena = Serial2.readString() ; 
    U3_Debug.println(cadena); //UART3
  }
}

void setup() {
  //TERMINAL RX0-TX0 -> SIM808
  Serial.begin(9600);
  //TERMINAL RX2-TX2 -> WATIMETRO
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2); //Dispositivo de bajada
  //TERMINAL RX3-TX3 -> TERMINAL
  U3_Debug.begin(9600, SWSERIAL_8N1, RX3, TX3, false, 256); // BADURATE del software serial de comunicacion
  // high speed half duplex, turn off interrupts during tx
  U3_Debug.enableIntTx(false);
  pinMode(MAX485_DE, OUTPUT);
  pinMode(led_esp32,OUTPUT);
}




void loop() {
  //cadena_amb_json = cadena_amb ; 
  doc["corriente"] = corriente;
  doc["voltaje"] = voltaje;
  doc["potencia"] = potencia;
  doc["energia"] = potencia;
  serializeJson(doc, json); 
  comandosAT();
}
