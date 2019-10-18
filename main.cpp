/*
Automatizacion Corp.
Codigo de prueba para ajustar varibles del sistema utilizando botones y escribiendo a rom 
Autor: Santos Michelena
ARDUINO: arduino uno
*/

#include <Internet.h>

const int hidrocapital = 18;
const int presionDesc = 25;
const int bomba1desc = 26;
const int bomba2desc = 23;


void setup(){
    Serial.begin(9600);
    pinMode(hidrocapital,INPUT);
    pinMode(bomba1desc, INPUT);
    pinMode(bomba2desc, INPUT);
    pinMode(presionDesc, INPUT_PULLUP);
}

void loop(){
   delay(1000);
   int presion = map(analogRead(presionDesc), 0, 4095, 0 ,100);
   String bomba1 = digitalRead(bomba1desc) ? "Bomba 1 activa" : "Bomba 1 desactiva" ;
   String bomba2 = digitalRead(bomba2desc) ? "Bomba 2 activa" : "Bomba 2 desactiva" ;

   Serial.println("presion actual del sistema: " + presion);
   Serial.println("Estado Bomba 1: " + bomba1);
   Serial.println("Estado Bomba 2: " + bomba2);
}