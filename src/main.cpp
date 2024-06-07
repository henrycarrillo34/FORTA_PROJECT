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



void setup() {
  //TERMINAL RX0-TX0 -> SIM808
  Serial.begin(9600);
  //TERMINAL RX2-TX2 -> WATIMETRO
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); //Dispositivo de bajada

  pinMode(MAX485_DE, OUTPUT);
  Serial2.begin(9600);
  Serial.begin(9600);
  pinMode(led_esp32,OUTPUT);
}




void loop() {


}
