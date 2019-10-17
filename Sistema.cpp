/*
Automatizacion Corp.
clase contenedora de sistema, sus variables y sus metodos para modificarlas
ARDUINO: esp32
SERVIDOR: raspberry pi 3 b+
AUTOR: Santos Michelena
*/

#include "Sistema.h"

//Direcciones de memoria en EEPROM.
const byte presionSistemaAdd = 0;
const byte bandaMuertaAdd = 2;
//const byte numBombasAdd = 4;
const byte timeDelayAdd = 6;

//PINS de botones en el arduino.
const int boton1 = 34;
const int boton2 = 35;
const int botonS = 36;
const int botonR = 37;

//PINS de lectura analogica
const int presionDescarga = 25;

//PINS de lectura digital
const int flow1 = 16;
const int flow2 = 17;
const int entradaHc = 18;
const int altaPresion = 19;
const int bajaPresion = 26;
const int bomba1desc = 27;
const int bomba2desc = 23;

//PINS de salida digital
const int bomba1 = 32;
const int bomba2 = 33;

//FLAGS de bombas
bool bomba1f;
bool bomba2f;

//CTOR lcd
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//CTOR
Sistema::Sistema(){
    //INPUTS digitales
    pinMode(boton1, INPUT);
    pinMode(boton2, INPUT);
    pinMode(botonS, INPUT);
    pinMode(botonR, INPUT);
    pinMode(flow1, INPUT);
    pinMode(flow2, INPUT);
    pinMode(entradaHc, INPUT);
    pinMode(altaPresion, INPUT);
    pinMode(bajaPresion, INPUT);
    pinMode(bomba1desc, INPUT);
    pinMode(bomba2desc, INPUT);

    //INPUTS digitales
    pinMode(presionDescarga, INPUT_PULLUP);

    //OUTPUTS
    pinMode(bomba1, OUTPUT);
    pinMode(bomba2, OUTPUT);

    bomba1f = true;
    bomba2f = false;
}

//SETTERS
void Sistema::setPresionSistema(byte presionSistema){EEPROM.write(presionSistemaAdd,presionSistema);}

void Sistema::setBandaMuerta(byte bandaMuerta){EEPROM.write(bandaMuertaAdd,bandaMuerta);}

//void Sistema::setNumBombas(byte numBombas){EEPROM.write(numBombasAdd, numBombas);}

void Sistema::SetTimeDelay(uint16_t timeDelay){EEPROM.write(timeDelayAdd, timeDelay);}

//GETTERS
byte Sistema::getPresion(){return EEPROM.read(presionSistemaAdd);}

byte Sistema::getBandaMuerta(){return EEPROM.read(bandaMuertaAdd);}

//byte Sistema::getNumBombas(){return EEPROM.read(numBombasAdd);}

uint16_t Sistema::getTimeDelay(){return EEPROM.read(timeDelayAdd);}

//METODOS

//AJUSTE VARIABLES CON BOTONES
bool Sistema::ajuste(){
    
  byte pres = getPresion();
  byte band = getBandaMuerta();
  
  if (!digitalRead(boton1) && !digitalRead(boton2))
    return false;
   else if (digitalRead(boton1)){
    Serial.println("Cambiando presion del sistema");
    delay(500);
    while(!digitalRead(boton1)){
      Serial.println("presion actual del sistema: " + String(pres));
      while(!digitalRead(botonS) && !digitalRead(botonR) && !digitalRead(boton1)){}
      if (digitalRead(botonS)){
        ++pres;
        delay(300);
      }
      if (digitalRead(botonR)){
        --pres;
        delay(300);
      }
    }
    delay(500);
    setPresionSistema(pres);
    Serial.println("ajuste finalizado");
    return true;
   }
   else if (digitalRead(boton2)){
    Serial.println("Cambiando presion de banda muerta");
    delay(500);
    while(!digitalRead(boton2)){
      Serial.println("presion de banda muerta actual: " + String(band));
      while(!digitalRead(botonS) && !digitalRead(botonR) && !digitalRead(boton2)){}
      if (digitalRead(botonS)){
        ++band;
        delay(300);
      }
      if (digitalRead(botonR)){
        --band;
        delay(300);
      }
    }
    delay(500);
    setBandaMuerta(band);
    Serial.println("ajuste finalizado");
    return true;
   }
   return false;
}

//BOMBEO
String Sistema::bombeo(){

    bomba1f = digitalRead(bomba1desc) ? bomba1f : false;
    bomba2f = digitalRead(bomba2desc) ? bomba2f : false;

    int pres = map(digitalRead(presionDescarga), 0, 100, 0, 4095);

    if(pres <= getBandaMuerta() && bomba1f){
        int cont = 0;
        while(pres <= getPresion() && cont < 3) {
            digitalWrite(bomba1, HIGH);
            delay(getTimeDelay());
            digitalWrite(bomba1, LOW);
            cont = digitalRead(flow1) ? cont : (cont + 1);
            pres = map(digitalRead(presionDescarga), 0, 100, 0, 4095);
        }
        if (cont == 2){
            bomba2f = true;
            bomba1f = false;
            return "B1FAIL";
        }
        else {
            bomba2f = true;
            bomba1f = false;
            return "B1SUCCESS";
        }
    
    }
    else if(pres <= getBandaMuerta() && bomba2f){
        int cont = 0;
        while(pres <= getPresion() && cont < 3) {
            digitalWrite(bomba2, HIGH);
            delay(getTimeDelay());
            digitalWrite(bomba2, LOW);
            cont = digitalRead(flow2) ? cont : (cont + 1);
            pres = map(digitalRead(presionDescarga), 0, 100, 0, 4095);
        }
        if (cont == 2){
            bomba1f = true;
            bomba2f = false;
            return "B2FAIL";
        }
        else {
            bomba1f = true;
            bomba2f = false;
            return "B2SUCCESS";
        }
    }
    else {
        return "..." ;
    }

}

//PARAR EL SISTEMA
bool Sistema::stop(){
    while(!digitalRead(boton1) || !digitalRead(boton2)){
        //printlcd("Sistema fuera de servicio")
        //delay(500);
        //printlcd("Para continuar, presione el boton a y el boton b al mismo tiempo");
        //delay(500);
    }
    if (digitalRead(boton1) && digitalRead(boton2)){
        //printlcd("Sistema a renaudar operacion normal");
        return true;
    }
    return true;
}

//CICLO
bool Sistema::ciclo(){
    String res1 = bombeo();
    String res2 = bombeo();

    if (((res1 == "B1FAIL") && (res2 == "B2FAIL")) || ((res1 = "B2FAIL") && (res2 == "B1FAIL"))){
        stop();
        return false;
    }
    else {
        return true;
    }
}

//IMPRIMIR EN LCD
void Sistema::printlcd(byte x, byte y, String mensj){
    //lcd es 16x,2y: x \in [0,15] , y \in [0,2]
    lcd.setCursor(x,y);
    lcd.print(mensj);
}

//DTOR
Sistema::~Sistema(){}
