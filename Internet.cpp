#include "Internet.h"

const char * page =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "  <title>Sistema de bombas - Automatizacion</title>"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "  <link rel=\"icon\" href=\"data:,\">"
    "  "
    "</head>"
    "<body>"
    "  <div class=\"main-container\">"
    "      <h1>Sistema de Bombas</h1>"
    "      <div name=\"wifi\">"
    "        <hr>"
    "        <h3>Configuración Wifi</h3>"
    "        <form action=\"/set_wifi\">"
    "          <label for=\"ssid\">SSID:</label>"
    "          <input class=\"input\" type=\"text\" id=\"ssid\"><br>"
    "          <label for=\"clave\">Contraseña:</label>"
    "          <input class=\"input\" type=\"password\" id=\"clave\"><br>"
    "          <input class=\"button\" type=\"submit\" value=\"Enviar\"/><br><br>"
    "        </form>"
    "      </div>"
    "      <div name=\"mqtt\">"
    "          <hr>"
    "          <h3>Configuración MQTT</h3>"
    "          <form action=\"/set_mqtt\">"
    "            <label for=\"host\">Host:</label>"
    "            <input class=\"input\" type=\"text\" id=\"host\"><br>"
    "            <label for=\"puerto\">Puerto:</label>"
    "            <input class=\"input\" type=\"text\" id=\"puerto\"><br>"
    "            <label for=\"usuario\">Usuario:</label>"
    "            <input class=\"input\" type=\"text\" id=\"usuario\"><br>"
    "            <label for=\"password\">Contraseña:</label>"
    "            <input class=\"input\" type=\"password\" id=\"password\"><br>"
    "            <input class=\"button\" type=\"submit\" value=\"Enviar\"/><br><br>"
    "          </form>"
    "          <hr>"
    "        </div>"
    "        <div name=\"bombas\">"
    "            <h3>Configuración Bombas</h3>"
    "            <form action=\"/set_bombas\">"
    "              <label for=\"presionsis\">Presion Sistema:</label>"
    "              <input class=\"input\" type=\"number\" id=\"presionsis\"><br>"
    "              <label for=\"presionban\">Presion Banda Muerta:</label>"
    "              <input class=\"input\" type=\"number\" id=\"presionban\"><br>"
    "              <label for=\"delay\">Time Delay:</label>"
    "              <input class=\"input\" type=\"number\" id=\"delay\"><br>"
    "              <input class=\"button\" type=\"submit\" value=\"Enviar\"/><br><br>"
    "            </form>"
    "          </div>"
    "  </div>"
    "</body>"
    "<style>"
    "html {"
    "  font-family: Arial, Helvetica, sans-serif;"
    "  display: inline-block;"
    "  margin: 0px auto;"
    "  text-align: center;"
    "  width: 500px;"
    "}"
    "form {"
    "    width: 500px;"
    "    font: normal bold 12px Verdana, Arial;"
    "}"
    "form label {"
    "    display: inline-block;"
    "    width: 250px;"
    "}"
    ".input {"
    "    display: inline-block;"
    "    width: 200px;"
    "    height: 20px;"
    "    margin-bottom: 10px;"
    "    border-radius: 5px;"
    "    border: 1px solid #39c;"
    "    padding-left: 10px;"
    "}"
    "label {"
    "  font-size: 1rem;"
    "}"
    "h1{"
    "  color: #0F3376;"
    "  padding: 2vh;"
    "}"
    ".button {"
    "  display: inline-block;"
    "  background-color: #008CBA;"
    "  border: none;"
    "  border-radius: 4px;"
    "  color: white;"
    "  padding: 5px 5px;"
    "  text-decoration: none;"
    "  font-size: 20px;"
    "  margin: 1px;"
    "  cursor: pointer;"
    "}"
    "</style>"
    "</html>"
    ;

String html = "<!DOCTYPE html> \
<html> \
<body> \
<center><h1>ESP32 Soft access point</h1></center> \
<center><h2>Web Server</h2></center> \
<form> \
<button name=\"LED0\" button style=\"color:green\" value=\"ON\" type=\"submit\">LED0 ON</button> \
<button name=\"LED0\" button style=\"color=red\" value=\"OFF\" type=\"submit\">LED0 OFF</button><br><br> \
<button name=\"LED1\" button style=\"color:green\" value=\"ON\" type=\"submit\">LED1 ON</button> \
<button name=\"LED1\" button style=\"color:red\" value=\"OFF\" type=\"submit\">LED1 OFF</button> \
</form> \
</body> \
</html>";



//CTOR DEFAULT
Internet::Internet(){}

//CONSTRUCTOR PARA OPERACION COMO ACCESS POINT  
Internet::Internet(String ssid, String pass){
    server = WiFiServer(80); //crea servidor en puerto 80
    ssid_ap = ssid;
    pass_ap = pass;
}

//CONSTRUCTOR PARA OPERACION DE ESTADO ESTACIONARIO
Internet::Internet(String ssid_hn, String pass_hn, String mqtt_user, String mqtt_pass, String mqtt_ip){
    this->ssid_hn = ssid_hn;
    this->pass_hn = pass_hn;
    this->mqtt_user = mqtt_user;
    this->mqtt_pass = mqtt_pass;
    this->mqtt_ip = mqtt_ip;
    this->cliente = PubSubClient(this->c);
    cliente.setServer(mqtt_ip.c_str(), 1883);
}


//CONFIGURA Y CREA EL ACCESS POINT
void Internet::crear_accessp(){
    Serial.print("creando punto de acceso …");
    WiFi.softAP(ssid_ap.c_str(), pass_ap.c_str());
    Serial.println("access point IP = ");
    Serial.println(WiFi.softAPIP());
    server.begin();
}

//PAGINA DE SERVIDOR WEB
void Internet::pagina(){
    WiFiClient client = server.available();
    if (client){
        String request = client.readStringUntil('\r');
        if (request.indexOf("LED0=ON") != -1){
            Serial.println("led0 on");
        }
        if (request.indexOf("LED0=OFF") != -1){
            Serial.println("led0 off");
        }
        if (request.indexOf("LED1=ON") != -1){
            Serial.println("led1 on");
        }
        if (request.indexOf("LED1=OFF") != -1){
            Serial.println("led1 off");
        } 
        client.print(html);
        request = "";
    }
}

//TUMBAR ACCESS POINT
void Internet::kill_accessp(){
    WiFi.softAPdisconnect(true);
    server.~WiFiServer();
}

//CONECTAR A RED LOCAL DE WIFI  
void Internet::conectar_WIFI(){
    delay(10);
    Serial.print("conectandose a: ");
    Serial.println(ssid_hn);

    //inicia proceso de conexion
    WiFi.begin(ssid_hn.c_str(), pass_hn.c_str());

    //si no conecta de una vez reintenta conexion cada medio segundo
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi conectado exitosamente");
    Serial.println("Direccion ip: ");
    Serial.println(WiFi.localIP());
}

void Internet::subscribir_MQTT(){
    cliente.subscribe("sistema-bombas/boost");
    cliente.subscribe("sistema-bombas/nivel-tanque");
}

//CONECTAR A SERVIDOR MQTT
void Internet::conectar_MQTT(){
    Serial.println("inicio a conexion mqtt");
    while(!cliente.connected()){
        Serial.println("El cliente no esta conectado, iniciar conexion");
        //id de cliente
        const char* clientId = "esp32_client";
        Serial.printf("clientId = %s \t mqtt_user = %s \t mqtt_pass = %s \t mqtt_ip = %s \n",
                       clientId, mqtt_user.c_str(), mqtt_pass.c_str(), mqtt_ip.c_str() );
        //intento de conectar
        if(cliente.connect(clientId ,mqtt_user.c_str(), mqtt_pass.c_str() )  ){
            //Subscripciones relevantes
            Serial.println("conexion a servidor mqtt exitosa");
            subscribir_MQTT();
        }
        else{
            //esperar 5 segundos antes de reintentar
            Serial.println("conexion a mqtt fallida");
            Serial.print("failed, rc=");
            Serial.print(cliente.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

//PUBLICAR MQTT 



Internet::~Internet(){}