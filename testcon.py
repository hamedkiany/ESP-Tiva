import socket
import time
# Configura la IP del ESP8266 y el puerto
esp8266_ip = '192.168.0.68'  # Cambia esto por la IP del ESP8266
port = 12345

# Crear un socket y conectar
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((esp8266_ip, port))
while(1):
    try:
        a  = input("select mode:")
        if(("exit" in a)):
            client_socket.close()
            break
        elif ("1" in a):
            # Enviar mensaje
            message = a
            client_socket.send(message.encode())

            # Recibir respuesta
            response = client_socket.recv(1024)
            print("Respuesta del ESP8266:", response.decode())
        elif("2" in a):
            try:
                while True:
                    # Intentar recibir mensaje desde el ESP8266
                    response = client_socket.recv(1024)
                    if response:
                        #print("Mensaje recibido desde ESP8266:", response.decode())
                        
                        print(response.decode())
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
    except:
        # Cerrar conexión
        client_socket.close()
