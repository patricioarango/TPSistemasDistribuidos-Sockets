import socket

def generar_usuario(sock):
    try:
        num = int(input("Ingresar Cantidad de Caracteres de Usuario (5 < x <= 15) > : "))
        message = f'usua{num}'.encode()
        sock.sendall(message)  
        response = sock.recv(4096)  
        print(f"{response.decode()}")
    except ValueError:
        print("Solo se pueden ingresar números.")
        generar_usuario(sock);

def generar_password(sock):
    try:
        num = int(input("Ingresar Cantidad de Caracteres del Password (8 <= x < 50) > : "))
        message = f'cont{num}'.encode()
        sock.sendall(message)  
        response = sock.recv(4096) 
        print(f"{response.decode()}")
    except ValueError:
        print("Solo se pueden ingresar números.")
        generar_password(sock);

def cerrar_socket(sock):
    print("Conexión Cerrada.")
    sock.sendall(b'clos')
    sock.close()
    
def menu():
    print("\nMenu:")
    print("1 - Generar Usuario")
    print("2 - Generar Contraseña")
    print("3 - Cerrar Conexión")
    return input("Elegir una opción (1, 2, 3): ")

def main():
    server_address = ('localhost', 5000) # puerto 5000 por defecto
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect(server_address)
            print("Connectado al servidor en el puerto :", server_address)

            while True:
                choice = menu()

                if choice == '1':
                    generar_usuario(s)
                elif choice == '2':
                    generar_password(s)
                elif choice == '3':
                    cerrar_socket(s)
                else:
                    print("Número inválido, por favor elegir entre 1, 2, o 3.")

    except ConnectionRefusedError:
        print("No se puede conectar al servidor. ")
    except Exception as e:
        print("Error:", e)

if __name__ == "__main__":
    main()
