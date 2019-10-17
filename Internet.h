/*
Automatizacion Corp.
clase que lidia con conexiones a internet
ARDUINO: esp32
SERVIDOR: raspberry pi 3 b+
AUTOR: Santos Michelena
*/

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Sistema.h"

#ifndef Internet_H
#define Internet_H

class Internet{

    private:
        WiFiClient c;
        PubSubClient cliente;

        //string que contiene pagina html completa
        String paginaHTML;

        //datos modo access point
        String ssid_ap;
        String pass_ap;

        //datos modo conectado a red local
        String ssid_hn;
        String pass_hn;

        //datos servidor mqtt
        String mqtt_ip;
        String mqtt_user;
        String mqtt_pass;

    public:
        //CTOR
        Internet();
        //DTOR
        ~Internet();
        //METODOS ACCESS POINT
        bool crear_accessp();
        bool kill_accessp();
        //METODOS WIFI
        bool conectar_WIFI();
        //METODOS MQTT
        bool conectar_MQTT();
        void publicar(String);
        String recibir();



};

#endif

