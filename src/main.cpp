/*
MODULO SIM808
*/
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
// ---- SOFTWARE TRIGGER GPRS ----
int PowerPin = 23; //prendido de placa SIM808

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

String servidor = "http://54.83.202.208:8080/api/v1/module-data/8";

//**************************
//        FUNCIONES
//**************************
void software_trigger(){
  pinMode(PowerPin, OUTPUT);
  //TURN OFF the GSM module -software switch instead Power Key
  digitalWrite(PowerPin, LOW);
  delay (2000);
  digitalWrite (PowerPin, HIGH);  
  delay (10000); //delay set for 10 seconds
}
//***********************//
//       GSM
//***********************//
void comandos_red_AT(){
  Serial2.println("AT"); /* Check Communication */
  delay(500);
  Serial2.println("AT+CSTT=\"https://wap.movistar.pe\",\"movistar@wap\",\"movistar\"");
  delay(500);
  Serial2.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(500);
  Serial2.println("AT+SAPBR=3,1,\"APN\",\"movistar.pe\"");
  delay(500);
  Serial2.println("AT+SAPBR=3,1,\"USER\",\"movistar@datos\"");
  delay(500);
  Serial2.println("AT+SAPBR=3,1,\"PWD\",\"Movistar\"");
  delay(500);
  Serial2.println("AT+SAPBR=1,1");
  delay(500);
  Serial2.println("AT+SAPBR=2,1");
  delay(500);
}


void comandosAT_JSON()
{
  Serial2.println("AT+HTTPINIT");
  delay(50);
  Serial2.println("AT+HTTPPARA=\"CID\",1");
  delay(500);
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
  Serial2.println("AT+HTTPREAD");
  delay(40);
  Serial2.println("AT+HTTPTERM");
  delay(50);
  Serial2.println("AT+SAPBR=0,1");
  delay(1000);
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
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2); 
  //TERMINAL RX3-TX3 -> TERMINAL
  U3_Debug.begin(9600, SWSERIAL_8N1, RX3, TX3, false, 256); // BADURATE del software serial de comunicacion
  // high speed half duplex, turn off interrupts during tx
  pinMode(LED, OUTPUT);
  U3_Debug.enableIntTx(false);
  pinMode(MAX485_DE, OUTPUT);
  pinMode(led_esp32,OUTPUT);
  U3_Debug.enableIntTx(false);
  software_trigger();
  //esperar
  delay(10000);
  comandos_red_AT();
}




void loop() {
  if ( Serial2.available() > 0 ){



  }
  //cadena_amb_json = cadena_amb ; 
  doc["corriente"] = corriente;
  doc["voltaje"] = voltaje;
  doc["potencia"] = potencia;
  doc["energia"] = potencia;
  serializeJson(doc, json); 
  comandos_red_AT();
  comandosAT_JSON();
}
