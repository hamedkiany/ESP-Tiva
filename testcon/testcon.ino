#include <ESP8266WiFi.h>

// Configuración de la red Wi-Fi
const char* ssid = "iPhone De Hamed15";
const char* password = "91219576717";
// Configuración de IP estática
IPAddress local_IP(172, 20, 10, 10);      // Cambia a la IP deseada
IPAddress gateway(172, 20, 10, 1);         // Cambia a la puerta de enlace de tu red
IPAddress subnet(255, 255, 255, 0);        // Máscara de subred de tu red
// Create data buffer for messages up to 40 characters
char msgBuffer [40];
volatile byte indx;
//String messageUART2="";
// Configuración del servidor
WiFiServer server(12345);

void setup() {
  Serial.begin(115200);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Error al configurar la IP estática");
  }
  Serial.swap();
  // Conexión a la red Wi-Fi
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado a la red Wi-Fi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar el servidor
  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  // Espera a que haya un cliente disponible
  //Serial.swap();
  
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Cliente conectado");
    String messageUART2 = handleData();
// Reducir tiempo de espera para evitar demoras
    client.setTimeout(50);  // Ajusta según tus necesidades
    // Mientras el cliente esté conectado
    while (client.connected()) {
      // Si hay datos disponibles del cliente
      if (client.available()) {
        messageUART2 = handleData();
        if(!(messageUART2 == "no"))
        {
          client.print("Respuesta de tiva\n"+messageUART2);
        }
        else
        {
//        String message = client.readStringUntil('\n');
//        Serial.print("Mensaje recibido: ");
//        Serial.println(message);

        // Enviar una respuesta al cliente
        //String response = "Mensaje recibido\n";
        client.print("\n");
        }
      }
    }

    // Cerrar la conexión cuando el cliente se desconecte
    client.stop();
    Serial.println("Cliente desconectado");
  }
}
// Función para enviar datos al dispositivo UART
String handleData() {
  String messageUART;
    // Listen for received UART data on Rx/Tx (D7/D8)
    if (Serial.available() > 0) {
        byte currentByte = Serial.read();
        if (indx < sizeof msgBuffer) {
            msgBuffer [indx++] = currentByte;

            // If end of message, send to dev computer
            if (currentByte == '\r') {
                messageUART = String(msgBuffer);
                Serial.swap();
                delay(200);
                Serial.println(messageUART);
                delay(200);
                Serial.swap();
                delay(200);
                indx = 0;
                return messageUART;
            }
        }
    }
    return "no";

}
