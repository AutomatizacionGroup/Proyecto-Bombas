/*
Automatizacion Corp.
Codigo de prueba para ajustar varibles del sistema utilizando botones y escribiendo a rom 
Autor: Santos Michelena
ARDUINO: arduino uno
*/

#include <EEPROM.h>
#include "Sistema.h"

const int bpresionSistema = 2;
const int bbandaMuerta = 3;
const int sumar = 4;
const int restar = 5;

Sistema sist(60,20,3000);

bool ajustar(Sistema sist){

  byte pres = sist.getPresion();
  byte band = sist.getBandaMuerta();
  
  if (!digitalRead(bpresionSistema) && !digitalRead(bbandaMuerta))
    return false;
   else if (digitalRead(bpresionSistema)){
    Serial.println("Cambiando presion del sistema");
    delay(500);
    while(!digitalRead(bpresionSistema)){
      Serial.println("presion actual del sistema: " + String(pres));
      while(!digitalRead(sumar) && !digitalRead(restar) && !digitalRead(bpresionSistema)){}
      if (digitalRead(sumar)){
        ++pres;
        delay(300);
      }
      if (digitalRead(restar)){
        --pres;
        delay(300);
      }
    }
    delay(500);
    sist.setPresionSistema(pres);
    Serial.println("ajuste finalizado");
    return true;
   }
   else if (digitalRead(bbandaMuerta)){
    Serial.println("Cambiando presion de banda muerta");
    delay(500);
    while(!digitalRead(bbandaMuerta)){
      Serial.println("presion de banda muerta actual: " + String(band));
      while(!digitalRead(sumar) && !digitalRead(restar) && !digitalRead(bbandaMuerta)){}
      if (digitalRead(sumar)){
        ++band;
        delay(300);
      }
      if (digitalRead(restar)){
        --band;
        delay(300);
      }
    }
    delay(500);
    sist.setBandaMuerta(band);
    Serial.println("ajuste finalizado");
    return true;
   }
   
}


void setup(){
    Serial.begin(9600);
    pinMode(bpresionSistema, INPUT);
    pinMode(bbandaMuerta, INPUT);
    pinMode(sumar, INPUT);
    pinMode(restar, INPUT);
}

void loop(){

  ajustar(sist);
  Serial.println("presion guardada en objeto: " + String(sist.getPresion()));
  Serial.println("presion guardada en EEPROM: " + String(EEPROM.read(100)));
  Serial.println("banda muerta guardada en objeto: " + String(sist.getBandaMuerta()));
  Serial.println("banda muerta guardada en EEPROM: " + String(EEPROM.read(50)));
  while(!digitalRead(bpresionSistema) && !digitalRead(bbandaMuerta)){}
   
}
