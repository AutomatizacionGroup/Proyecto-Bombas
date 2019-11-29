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
const byte timeDelayAdd = 4;
const byte user_hnAdd = 20;
const byte pass_hnAdd = 40;
const byte mqtt_userAdd = 60;
const byte mqtt_passAdd = 80;
const byte mqtt_ipAdd = 100;

//PINS de botones en el arduino.
const int boton1 = 34;
const int boton2 = 35;
const int botonS = 36;
const int botonR = 39;

//PINS de lectura analogica
const int presionDescarga = 33;

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
const int bomba2 = 25;

//BYTES a acceder en EEPROM
#define EEPROM_SIZE 200


//CTOR lcd
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//CTOR
Sistema::Sistema(){

    //Inializa pantalla lcd
    lcd.init(21,22);
    lcd.begin(16,2,	LCD_5x8DOTS);
    lcd.backlight();
    lcd.clear();

    //Setea cantidad de memoria eeprom a acceder
    EEPROM.begin(EEPROM_SIZE);

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

    this->timeDelay = 3000;
    bomba1f = true;
    bomba2f = false;
}

//SETTERS
void Sistema::setPresionSistema(byte presionSistema){
    EEPROM.write(presionSistemaAdd,presionSistema);
    EEPROM.commit();
}

void Sistema::setBandaMuerta(byte bandaMuerta){
    EEPROM.write(bandaMuertaAdd,bandaMuerta);
    EEPROM.commit();
}

void Sistema::setTimeDelay(uint16_t timeDelay){
    EEPROM.writeUShort(timeDelayAdd, timeDelay);
    EEPROM.commit();
    this->timeDelay = timeDelay;
}

void Sistema::setUser_hn(String user){
    EEPROM.writeString(user_hnAdd,user);
    EEPROM.commit();
}

void Sistema::setPass_hn(String pass){
    EEPROM.writeString(pass_hnAdd, pass);
    EEPROM.commit();
}

void Sistema::setMqtt_user(String user){
    EEPROM.writeString(mqtt_userAdd, user);
    EEPROM.commit();
}

void Sistema::setMqtt_pass(String pass){
    EEPROM.writeString(mqtt_passAdd, pass);
    EEPROM.commit();
}

void Sistema::setMqtt_ip(String ip){
    EEPROM.writeString(mqtt_ipAdd, ip);
    EEPROM.commit();
}

//GETTERS
byte Sistema::getPresion(){return EEPROM.read(presionSistemaAdd);}

byte Sistema::getBandaMuerta(){return EEPROM.read(bandaMuertaAdd);}

bool Sistema::getBajaPresion(){return digitalRead(bajaPresion);}

bool Sistema::getAltaPresion(){return digitalRead(altaPresion);}

bool Sistema::getEntradaHc(){return digitalRead(entradaHc);}

String Sistema::getUser_hn(){return EEPROM.readString(user_hnAdd);}

//byte Sistema::getNumBombas(){return EEPROM.read(numBombasAdd);}

uint16_t Sistema::getTimeDelay(){
    //return EEPROM.read(timeDelayAdd);
    return this->timeDelay;
}

//METODOS

//AJUSTE VARIABLES CON BOTONES
bool Sistema::ajuste(){

    byte pres = getPresion();
    byte band = getBandaMuerta();
  
    if (!digitalRead(boton1) && !digitalRead(boton2))
        return false;
    else if (digitalRead(boton1)){
        //mensaje a monitor serial
        Serial.println("Cambiando presion del sistema");
        //mensaje a pantalla lcd
        lcd.clear();
        printlcd(0,0, "Inicio cambio", false);
        delay(500);
        while(!digitalRead(boton1)){

            //mensaje a monitor serial
            Serial.println("presion actual del sistema: " + String(pres));
            // |P||S||:| 
            printlcd(0,1, "PS:", false);
            // |P||S||:||2||3| (23 = ejemplo)
            printlcd(3,1, String(pres), false);

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
        // |F||I||N|
        printlcd(0,0, "FIN", true);
        return true;
    }
    else if (digitalRead(boton2)){
        //mensaje a monitor serial
        Serial.println("Cambiando presion de banda muerta");
        //mensaje a pantalla lcd
        lcd.clear();
        printlcd(0,0, "Inicio Cambio BM:", false);
        delay(500);

        while(!digitalRead(boton2)){
            Serial.println("presion de banda muerta actual: " + String(band));
            //|B||M||:||1||5| (15 = ejemplo)
            printlcd(0,1, "BM:", false);
            printlcd(3,1, String(band), false);
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
    printlcd(0,0, "FIN", true);
    return true;

   }
   return false;
}

byte Sistema::getCurrentPres(){
   return (map(analogRead(presionDescarga), 0 , 4095, 0, 100));
}

//BOMBEO
String Sistema::bombeo(){

    bool bomba1d = digitalRead(bomba1desc);
    bool bomba2d = digitalRead(bomba2desc);

    if((getCurrentPres() <= getBandaMuerta()) && (bomba1f || !bomba2d)){
        int cont = 0;
        while(getCurrentPres() <= getPresion() && cont < 3) {
            digitalWrite(bomba1, HIGH);
            bomba1ON = true;
            Serial.println("bomba 1 prendida");
            status();
            delay(getTimeDelay());
            digitalWrite(bomba1, LOW);
            bomba1ON = false;
            status();
            cont = digitalRead(flow1) ? cont : (cont + 1);
            Serial.println(String(cont));
        }
        if (cont == 3){
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
    else if( (getCurrentPres() <= getBandaMuerta()) && (bomba2f || !bomba1d) ){
        int cont = 0;
        while(getCurrentPres() <= getPresion() && cont < 3) {
            digitalWrite(bomba2, HIGH);
            bomba2ON = true;
            status();
            Serial.println("bomba 2 prendida");
            delay(getTimeDelay());
            digitalWrite(bomba2, LOW);
            bomba2ON = false;
            status();
            cont = digitalRead(flow2) ? cont : (cont + 1);
            Serial.println(String(cont));
        }
        if (cont == 3){
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
        lcd.clear();
        printlcd(0,0, "FALLA TOTAL", false);
        printlcd(0,1, "PRESS 1&2", false);
        delay(500);
    }
    if (digitalRead(boton1) && digitalRead(boton2)){
        delay(300);
        lcd.clear();
        printlcd(0,0, "SISTEMA", false);
        printlcd(0,1, "RESETEADO", true);
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
void Sistema::printlcd(byte x, byte y, String mensj, bool clear){
    //lcd es 16x,2y: x \in [0,15] , y \in [0,2]
    if (clear){
        lcd.setCursor(x,y);
        lcd.print(mensj.c_str());
        delay(1000);
        lcd.clear();
    }
    else{
        lcd.setCursor(x,y);
        lcd.print(mensj.c_str());
    }
}

void Sistema::status(){
    printlcd(0,0, "Estado:", false);
    printlcd(9,0, "PA:", false);
    printlcd(13,0, " ", false);
    printlcd(14,0, " ", false);
    printlcd(12,0, String(getCurrentPres()), false);

    printlcd(0,1, "B1:", false);
    printlcd(9,1, "B2:", false);

    if (bomba1ON){
        printlcd(5,1, " ", false);
        printlcd(3,1, "ON", false);
    }
    else if (!bomba1ON){
        printlcd(3,1, "OFF", false);
    }
    if (bomba2ON){
        printlcd(14, 1, " ", false);
        printlcd(12,1, "ON", false);
    }
    else if(!bomba2ON){
        printlcd(12,1, "OFF", false);
    }

}

//DTOR
Sistema::~Sistema(){}
