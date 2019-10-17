/*
Automatizacion Corp.
clase contenedora de sistema, sus variables y sus metodos para modificarlas
ARDUINO: esp32
SERVIDOR: raspberry pi 3 b+
AUTOR: Santos Michelena
*/

/*
PARA TODO EL SIGUIENTE DOCUMENTO, MEMORIA EEPROM = MEMORIA FLASH
*/

#include <Arduino.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

#ifndef Sistema_H
#define Sistema_H

class Sistema{

    /* Variables no son necesarias. todo se escribe a memoria EEPROM.
    private:
        byte presionSistema;
        byte bandaMuerta;
        byte numBombas;
        int timeDelay;
    */

    public:
        //SETTERS: escriben a EEPROM
        void setPresionSistema(byte);
        void setBandaMuerta(byte);
        //void setNumBombas(byte);
        void SetTimeDelay(uint16_t);
        //GETTERS: leen de EEPROM
        byte getPresion();
        byte getBandaMuerta();
        //byte getNumBombas();
        uint16_t getTimeDelay();
        //METODOS DE SISTEMA:
        bool ajuste();
        String bombeo();
        bool stop();
        bool ciclo();
        void printlcd(byte, byte, String);
        //CTOR:
        Sistema();
        //DTOR:
        virtual ~Sistema();
};

#endif
