/*
Automatizacion Corp.
clase contenedora de sistema, sus variables y sus metodos para modificarlas
ARDUINO: esp32
SERVIDOR: raspberry pi 3 b+
AUTOR: Santos Michelena
*/

#include <Arduino.h>
#include <EEPROM.h>

#ifndef Sistema_H
#define Sistema_H

class Sistema{
    private:
        byte presionSistema;
        byte bandaMuerta;
        byte timeDelay;
    
    public:
        Sistema(byte, byte, byte);
        void setPresionSistema(byte);
        void setBandaMuerta(byte);
        byte getPresion();
        byte getBandaMuerta();
        void serialPrintSystem();
        virtual ~Sistema();
};

#endif
