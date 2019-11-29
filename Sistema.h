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

    private:
        uint16_t timeDelay;
        /*
        String user_hn;
        String pass_hn;
        String mqtt_user;
        String mqtt_pass;
        String mqtt_ip;
        */
       
        //FLAGS de bombas
        bool bomba1f;
        bool bomba2f;
        bool bomba1ON;
        bool bomba2ON;

    public:

        //SETTERS: escriben a EEPROM
        void setPresionSistema(byte);
        void setBandaMuerta(byte);
        void setTimeDelay(uint16_t);
        void setUser_hn(String);
        void setPass_hn(String);
        void setMqtt_user(String);
        void setMqtt_pass(String);
        void setMqtt_ip(String);

        //GETTERS que leen de EEPROM
        byte getPresion();
        byte getBandaMuerta();
        uint16_t getTimeDelay();
        String getUser_hn();
        String getPass_hn();
        String getMqtt_user();
        String getMqtt_pass();
        String getMqtt_ip();

        //GETTERS que leen de entradas digitales
        bool getBajaPresion();
        bool getAltaPresion();
        bool getEntradaHc();

        //GETTERS que leen de entradas analogicas
        byte getCurrentPres();

        //METODOS DE SISTEMA:
        bool ajuste(); //ajusta setpoints
        String bombeo(); //prende y apaga bombas cuando es necesario
        bool stop(); //para el sistema en caso de una falla
        bool ciclo(); //maneja todo el ciclo fundamental del sistema
        void printlcd(byte, byte, String, bool); //imprime a pantalla lcd
        void status(); //imprime estado actual de operacion (simplificado) a pantalla lcd

        //CTOR:
        Sistema();
        //DTOR:
        virtual ~Sistema();
};

#endif
