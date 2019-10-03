/*
Automatizacion Corp.
clase contenedora de sistema, sus variables y sus metodos para modificarlas
ARDUINO: esp32
SERVIDOR: raspberry pi 3 b+
AUTOR: Santos Michelena
*/

#include "Sistema.h"
#include <EEPROM.h>

uint8_t presionSistemaAdd = 100;
uint8_t bandaMuertaAdd = 50;
uint8_t timeDelayAdd = 20;

Sistema::Sistema(byte presionSistema, byte bandaMuerta, byte timeDelay){
    this->presionSistema = presionSistema;
    this->timeDelay = timeDelay;
    this->bandaMuerta = bandaMuerta;

    EEPROM.write(presionSistemaAdd,presionSistema);
    EEPROM.write(bandaMuertaAdd,bandaMuerta);
    EEPROM.write(timeDelayAdd,timeDelay);    
}

void Sistema::setPresionSistema(byte presionSistema){
    this->presionSistema = presionSistema;
    EEPROM.write(presionSistemaAdd,presionSistema);
}

void Sistema::setBandaMuerta(byte bandaMuerta){
    this->bandaMuerta = bandaMuerta;
    EEPROM.write(bandaMuertaAdd,bandaMuerta);
}

byte Sistema::getPresion(){return this-> presionSistema;}

byte Sistema::getBandaMuerta(){return this-> bandaMuerta;}

void Sistema::serialPrintSystem(){
    Serial.println("La presion del sistema es: " + getPresion());
    Serial.println("La banda muerta es: " + getBandaMuerta());
}

Sistema::~Sistema(){}
