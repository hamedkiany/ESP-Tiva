import socket
import struct  # Para decodificar el struct binario
import time

# Configura la IP del ESP8266 y el puerto
esp8266_ip = '192.168.0.68'  # Cambia esto por la IP del ESP8266
port = 12345

# Crear un socket y conectar
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((esp8266_ip, port))

# Definir el formato del struct que esperas recibir (ejemplo: 'i f' -> int, float)
struct_format = 'i i i i i i i i'  # Cambia según el formato del struct
struct_size = struct.calcsize(struct_format)

while True:
    try:
        a = input("select mode:")
        if "exit" in a:
            client_socket.close()
            break
        elif "1" in a:
            # Enviar mensaje
            message = a
            client_socket.send(message.encode())

            # Recibir respuesta (puede ser texto en este caso)
            response = client_socket.recv(1024)
            print("Respuesta del ESP8266:", response.decode())
        elif "2" in a:
            try:
                while True:
                    # Intentar recibir el tamaño completo del struct desde el ESP8266
                    response = client_socket.recv(struct_size)
                    if response:
                        # Asegúrate de recibir el tamaño completo del struct
                        if len(response) == struct_size:
                            # Decodifica el struct según el formato (int, float)
                            data = struct.unpack(struct_format, response)
                            #print("Datos recibidos desde ESP8266:", data)
                            print(data)
                        else:
                            #print("Error: Tamaño de struct incompleto")
                            pass
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
    except Exception as e:
        print("Error:", e)
        # Cerrar conexión
        client_socket.close()
