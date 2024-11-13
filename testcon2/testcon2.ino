#include <ESP8266WiFi.h>

// Configuración de la red Wi-Fi
const char* ssid = "Avatel_34";
const char* password = "9121957671";
// Configuración de IP estática
IPAddress local_IP(192, 168, 0, 169);      // Cambia a la IP deseada
IPAddress gateway(192, 168, 0, 1);         // Cambia a la puerta de enlace de tu red
IPAddress subnet(255, 255, 255, 0);        // Máscara de subred de tu red
// Create data buffer for messages up to 40 characters
char msgBuffer [40];
volatile byte indx;
//String messageUART2="";
// Configuración del servidor
WiFiServer server(12345);
void setup() {
    Serial.begin(115200);
    Serial.swap();  // Cambia Rx/Tx a D7/D8 para comunicación UART
    if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Error al configurar la IP estática");
    }

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConectado a Wi-Fi con IP:");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("Servidor TCP iniciado en el puerto 12345");
}

void loop() {
    // Revisa si hay un cliente TCP conectado
    WiFiClient client = server.available();
    if (client) {
        Serial.println("Cliente conectado");

        client.setTimeout(50);  // Tiempo de espera breve para evitar demoras

        while (client.connected()) {
            // Escucha datos recibidos en UART
            if (Serial.available() > 0) {
                byte currentByte = Serial.read();
                if (indx < sizeof msgBuffer - 1) {
                    msgBuffer[indx++] = currentByte;

                    // Si el mensaje termina con '\r', envíalo al cliente
                    if (currentByte == '\r') {
                        msgBuffer[indx] = '\0';  // Termina la cadena de texto
                        String message = String(msgBuffer);

                        // Envía el mensaje UART al cliente TCP
                        client.print(message);
                        indx = 0;  // Reinicia el índice del buffer
                    }
                }
            }
        }

        // Cierra la conexión cuando el cliente se desconecta
        client.stop();
        Serial.println("Cliente desconectado");
    }
}