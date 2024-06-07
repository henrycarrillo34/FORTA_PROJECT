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
//---TX2-RX2 (Debug- Watimetro)
#define RXp2 16
#define TXp2 17
//---TX3-RX3 (STM32)
#define RX3 (13)
#define TX3 (12)
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

//OTRA FUNCION PARA CONVERTIR DE HEXADECIMAL A FLOTANTE
float ieee_float(uint32_t f)
{
    static_assert(sizeof(float) == sizeof f, "float has a weird size.");
    float ret;
    std::memcpy(&ret, &f, sizeof(float));
    return ret;
}


unsigned long timerDelay_0 = 1000;
unsigned long timerDelay_wifi = 5000;
unsigned long lastTime_0 = 0;   // para millis()

void setup() {
  pinMode(MAX485_DE, OUTPUT);
  Serial2.begin(9600);
  Serial.begin(9600);
  pinMode(led_esp32,OUTPUT);
}




void loop() {


}
