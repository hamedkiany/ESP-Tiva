#include <ESP8266WiFi.h>

// Configuración de la red Wi-Fi
const char* ssid = "Avatel_34";
const char* password = "9121957671";
// Configuración de IP estática
IPAddress local_IP(172, 20, 10, 10);      
IPAddress gateway(172, 20,10, 1);         
IPAddress subnet(255, 255, 255, 0);        

// Tamaño del struct (por ejemplo, 8 bytes para un entero y un float)
const size_t structSize = 8;
byte msgBuffer[structSize];
size_t indx = 0;

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

        client.setTimeout(50);

        while (client.connected()) {
            // Escucha datos recibidos en UART
            if (Serial.available() > 0) {
                msgBuffer[indx++] = Serial.read();  // Lee byte por byte en el buffer

                // Cuando se ha recibido el tamaño completo del struct, envíalo
                if (indx >= structSize) {
                    client.write(msgBuffer, structSize);  // Envía el struct completo al cliente TCP
                    indx = 0;  // Reinicia el índice del buffer
                }
            }
        }

        // Cierra la conexión cuando el cliente se desconecta
        client.stop();
        Serial.println("Cliente desconectado");
    }
}
