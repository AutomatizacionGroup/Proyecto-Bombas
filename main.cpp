#include <Arduino.h>
#include <Sistema.h>
#include <WiFi.h>
#include <PubSubClient.h>

Sistema sist;
WiFiClient c;
PubSubClient cliente(c);
WiFiServer server(80);

const char* user_ap = "sistema-bombas";
const char* pass_ap = "automatizacion";

long lastMsg = 0;

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

void conectar_WIFI(String ssid, String password){

  delay(10);
  Serial.print("conectandose a: ");
  Serial.println(ssid.c_str());

  //inicia proceso de conexion
  WiFi.begin(ssid.c_str(), password.c_str());

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

void conectar_MQTT(String mqtt_user, String mqtt_pass, String mqtt_ip){
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
      cliente.subscribe("sistema-bombas/boost");
      cliente.subscribe("sistema-bombas/ajuste");
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

void callback(char* topic, byte* payload, unsigned int length){

  Serial.print("ha llegado mensaje en el topico: ");
  Serial.print(topic);
  Serial.print(". Mensaje: ");

  String mensajeTemp;

  //construye string a partir de mensaje y lo imprime
  for(int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
    mensajeTemp += (char)payload[i];
  }
  Serial.println(mensajeTemp);

  //codigo de respuesta a mensaje
  if (String(topic) == "boost/sistema-bombas"){
    if (mensajeTemp == "boost"){
        sist.setPresionSistema(sist.getPresion() + 15);
        sist.setBandaMuerta(sist.getBandaMuerta() + 15);
    } 
  }
  else if (String(topic) == "ajuste"){
    //ajustar setpoints
    Serial.println("ajuste de setpoints mediante internet comenzado");
  }
}

void pagina_ap(){
  WiFiClient client = server.available();
  if (client)
  {
    String request = client.readStringUntil('\r');
    client.print(page);
    request = "";
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sist = Sistema();
  conectar_WIFI("bio","biomic1044");
  cliente.setServer("192.168.1.221", 1883);
  cliente.setCallback(callback);
  /*
  WiFi.softAP(user_ap,pass_ap);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();
  */

}

void loop() {
  
  if(!cliente.connected()){
    conectar_MQTT("sistema-bomas","automatizacion","192.168.1.221");
  }
  cliente.loop();
  

  //pagina_ap();

  long now = millis();
  if (now - lastMsg > 5000){
    lastMsg = now;

    cliente.publish("sistema-bombas/presion", String(sist.getCurrentPres()).c_str());
    cliente.publish("sistema-bombas/baja-presion", String(sist.getBajaPresion()).c_str());
    cliente.publish("sistema-bombas/alta-presion" , String(sist.getAltaPresion()).c_str());
    cliente.publish("sistema-bombas/entrada-hc", String(sist.getEntradaHc()).c_str());

  }
  

  sist.status();
  sist.ciclo();
  sist.ajuste();

  
}
