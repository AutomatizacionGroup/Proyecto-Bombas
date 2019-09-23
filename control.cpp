/*
Automatizacion Corp.
Codigo de algoritmo control de bombas usando tecnologia IoT
Arduino: ESP32
Servidor: Raspberry Pi 3 B+
Autor: Santos Michelena
*/

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
todas las entradas y salidas estan definidas para un ESP32 
TOMANDO EN CUENTA EL OUTPUT DE DICHOS PINES EN BOOT	
*/

//ENTRADAS DIGITALES:
//flujo switch de bomba 1
const int flow1 = 16;

//flujo switch de bomba 2
const int flow2 = 17;

//entrada hidrocapital
const int EntradaHidrocapital = 18;

//Alta presion de salida de las bombas
const int AltaPresionSistema = 19;

//Baja presion de salida de las bombas
const int BajaPresionSistema = 26;

//bomba 1 en servicio/mantenimiento
const int bomba1mantenimiento = 27;

//bomba 2 en servicio/mantenimiento
const int bomba2mantenimiento = 23;

//boton para cambiar setpoints
const int botonSistema = 37;

//ENTRADAS ANALOGICAS:
//presion de descarga de las bombas
const int presionDescarga = 25;

//potenciometro de presion del sistema (pin es input only)
const int potPresionSistema = 34;

//potenciometro de presion de banda muerta (pin es input only)
const int potBandaMuerta = 35;

//potenciometro de delay de tiempo despues de prender una bomba (pin es input only)
const int potTimeDelay = 36;

//SALIDAS DIGITALES:
//BOMBA 1
const int bomba1 = 32;

//BOMBA 2
const int bomba2 = 33;

//I2C SDA (pantalla)
const int sda = 21;

//I2C SCL (pantalla)
const int scl = 22;

//VARIABLES DEL PROGRAMA:

bool entroAgua;
bool cambiarSetpoints;
bool b1,b2,b1Flag,b2Flag;

//objeto contenedor de setpoints del sistema
struct Setpoints{
    int presionSistema;
    int bandaMuerta;
    int timeDelay;
};

//FUNCIONES DEL PROGRAMA:

void imprimirLcd(String mensaje){
    //codigo para imprimir en la pantalla lcd
}

bool entradaAgua(){
    return digitalRead(EntradaHidrocapital);
}

bool boton(){
    return digitalRead(botonSistema);
}

void ajusteSetpoints(Setpoints sp){
    while(boton()){
        sp.presionSistema = map(digitalRead(potPresionSistema), 0, 4095, 0, 100);
        sp.bandaMuerta = map(digitalRead(potBandaMuerta), 0, 4095, 0, 100);
        sp.timeDelay = map(digitalRead(potTimeDelay), 0, 4095, 0, 100);
    }
    if (!entroAgua)
            sp.presionSistema = 50; //ejemplo
}

int presionDS(){
    return map(analogRead(presionDescarga), 0, 150, 0 , 100);
}

//devuelve 1 si la bomba esta en servicio
bool bomba1estado(){
    return digitalRead(bomba1mantenimiento);
}

//devuelve 1 si la bomba esta en servicio
bool bomba2estado(){
    return digitalRead(bomba2mantenimiento);
}

bool bombeo(Setpoints sp){
    int cont = 0;
    while(presionDS() != sp.presionSistema && cont < 3){
        if (b1 && !bomba1estado()){
            digitalWrite(bomba1, HIGH);
            delay(sp.timeDelay);
            if (!digitalRead(flow1)){++cont;}
        }
        else if (b2 && !bomba2estado()){
            digitalWrite(bomba2, HIGH);
            delay(sp.timeDelay);
            if (!digitalRead(flow1)){++cont;}
        }
    }
    //Escenarios de falla:
    if (!b1Flag && !b2Flag){
        do{
        imprimirLcd("¡Falla total!, pulsar boton para reiniciar");
        }
        while(!boton());
        b1 = true;
        b2 = false;
        b1Flag = true;
        b2Flag = true;
        return false;
    }
    if (b1 && cont == 2){
        imprimirLcd("¡La bomba 1 esta fuera de servicio!");
        b1 = false;
        b1Flag = false;
        b2 = true;
    }
    else if (b2 && cont == 2){
        imprimirLcd("¡La bomba 2 esta fuera de servicio");
        b2 = false;
        b2Flag = false;
        b1 = true;
    }
    if (b1){
        b2 = true;
        b1 = false;
    }
    else {
        b1 = true;
        b2 = false;
    }
    return true;
}

void setup(){

    //SETUP ENTRADAS DIGITALES
    pinMode(flow1, INPUT);
    pinMode(flow2, INPUT);
    pinMode(EntradaHidrocapital, INPUT);
    pinMode(AltaPresionSistema, INPUT);
    pinMode(BajaPresionSistema, INPUT);
    pinMode(bomba1mantenimiento, INPUT);
    pinMode(bomba2mantenimiento, INPUT);
    pinMode(botonSistema, INPUT);

    //ENTRADAS ANALOGICAS:
    pinMode(presionDescarga, INPUT_PULLUP);
    pinMode(potPresionSistema, INPUT_PULLUP);
    pinMode(potBandaMuerta, INPUT_PULLUP);
    pinMode(potTimeDelay, INPUT_PULLUP);

    //SALIDAS DIGITALES:
    pinMode(bomba1, OUTPUT);
    pinMode(bomba2, OUTPUT);
    /*
    pinMode(sda, OUTPUT); [es posible que estos pines no haya que declararlos]
    pinMode(scl, OUTPUT);
    */

   //para empezar a bombear con la bomba 1
   b1 = true;

}


void loop(){

    Setpoints  sp;

    entroAgua = entradaAgua();
    ajusteSetpoints(sp);
    bombeo(sp);
}