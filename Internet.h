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
    public:
    
    WiFiServer server;
    WiFiClient c;
    PubSubClient cliente;

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

    //METODOS

    //CTOR
    Internet(String, String);
    Internet(String, String, String, String, String);
    //DTOR
    ~Internet();
    //METODOS ACCESS POINT
    void crear_accessp(String, String);
    void pagina();
    void kill_accessp();
    //METODOS WIFI
    void conectar_WIFI();
    //METODOS MQTT
    void conectar_MQTT();
    void subscribir_MQTT();
    void publicar(char, String);
    String recibir();



};

#endif

