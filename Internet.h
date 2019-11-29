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

#ifndef Internet_H
#define Internet_H

class Internet{

    private:

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

    WiFiServer server;
    WiFiClient c;
    PubSubClient cliente;

    //METODOS

    //METODOS ACCESS POINT
    void crear_accessp();
    void pagina();
    void kill_accessp();

    //METODOS WIFI
    void conectar_WIFI();

    //METODOS MQTT
    void conectar_MQTT();
    void subscribir_MQTT();
    void publicar(char, String);
    //void recibir(char*, byte*, unsigned int);

    //CTOR
    Internet();
    Internet(String ssid, String pass);
    Internet(String ssid_hn, String pass_hn, String mqtt_user, String mqtt_pass, String mqtt_ip);
    //DTOR
    ~Internet();


};

#endif

