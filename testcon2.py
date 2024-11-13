import socket

# Configura la IP del ESP8266 y el puerto
esp8266_ip = '172.20.10.10'  # Cambia esto por la IP del ESP8266
port = 12345

# Crear un socket y conectar
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((esp8266_ip, port))

print("Conectado al ESP8266 y en espera de mensajes...")

try:
    while True:
        # Intentar recibir mensaje desde el ESP8266
        response = client_socket.recv(1024)
        if response:
            print("Mensaje recibido desde ESP8266:", response.decode())
        else:
            print("Conexión cerrada por el servidor, reconectando...")
            client_socket.close()
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client_socket.connect((esp8266_ip, port))
            print("Reconectado al ESP8266")
except KeyboardInterrupt:
    print("\nFinalizando conexión por solicitud del usuario.")
finally:
    # Cerrar conexión al terminar
    client_socket.close()
    print("Conexión cerrada.")
