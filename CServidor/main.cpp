#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

static const char vocales[] = "aeiou";
static const char consonantes[] = "bcdfghjklmnpqrstvwxyz";
static const char abecedario[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";



class Servidor{
    public:
    socklen_t server,client;
    sockaddr_in serverAddr,clientAddr;
    char buffer[4096];
    int clientSocket;
    int Puerto = 5000;
    int servidor;
    Servidor()
    {
        servidor = socket(AF_INET, SOCK_STREAM, 0);
        if(servidor < 0){
            cerr << "Error al crear el socket" << endl;
            exit(1);
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(Puerto);
        inet_pton(AF_INET, "0.0.0.0", &serverAddr.sin_addr);
        string msj = "/* INICIA SERVIDOR */";
        cout << msj << endl;
        bind(servidor, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    }

    bool Conectar()
    {
        int escuchar = listen(this->servidor,0);
        string msj2 = " Socket creado en puerto: " + to_string(Puerto);
        cout << msj2 << endl;
        socklen_t clientSize = sizeof(client);  //aca decia clientAddr
        clientSocket = accept(servidor, (struct sockaddr*)&clientAddr, &clientSize);
        char host[NI_MAXHOST];      // Nombre del cliente
        char service[NI_MAXSERV];   // Puerto en el que se conecta
        if(getnameinfo((struct sockaddr*)&clientAddr, sizeof(clientAddr), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
            cout << host << " cliente conectado en puerto " << service << endl;
        }
        if(clientSocket < 0)
        {
            return false;
        } else {
            return true;
        }
    }

    int Recibir()
    {
        char buf[4096];
        memset(buf, 0, 4096);
        int bytesRecv = recv(this->clientSocket,buf,sizeof(buf), 0);
        std::string str = buf;
        cout << " <ClienteDice>: " << string(buf,0,bytesRecv) << endl;
        string mensaje = str;
        if(mensaje != "")
        {
            int tareaParaHacer = this->analizarComando(mensaje);
            switch(tareaParaHacer)
                {
                    case 1:
                        this->generarUsuario(mensaje);
                        break;
                    case 2:
                        this->generarPassword(mensaje);
                        break;
                    case 3:
                        this->CerrarSocket();
                        this->Conectar();
                        break;
                }
        }

    }

    int analizarComando(string msj)
    {
        int res = 0;
        string comando = msj.substr(0,4);
        if(comando == "usua")
        {
            res = 1;
        }
        if(comando == "cont")
        {
            res = 2;
        }
        if(comando == "clos")
        {
            res = 3;
        }
        return res;
    }

    bool longitudUsuarioCorrecta(int cantidad_caracteres)
    {
        bool res = false;
        if(cantidad_caracteres < 5  || cantidad_caracteres > 15)
        {
            string respuesta = "Error desde el Servidor: La longitud del usuario no puede ser menor a 5 ni mayor a 15.";
            cerr << "La longitud del usuario no puede ser menor a 5 ni mayor a 15." << endl;
            Enviar(respuesta);
        } else {
            res = true;
        }
        return res;
    }

    void generarUsuario(string mensaje)
    {
        std::string operacion = mensaje.substr(4, mensaje.length());
        int cantidad_caracteres = atoi(operacion.c_str());
        if(longitudUsuarioCorrecta(cantidad_caracteres))
        {
            //si es par inicia vocal,
            bool iniciaVocal = false;
            if((generateRandomNumber(0) % 2)== 0)
            {
                iniciaVocal = true;
            }
            std::string usuario;
            for(int i=0 ; i<cantidad_caracteres;i++)
            {
                if(iniciaVocal)
                {
                    usuario += (i%2 == 0) ? vocales[generateRandomNumber(5)] : consonantes[generateRandomNumber(21)];
                } else {
                    usuario += (i%2 == 0) ? consonantes[generateRandomNumber(21)] : vocales[generateRandomNumber(5)];
                }
            }

            string respuesta = "Usuario Random Generado (si empieza vocal sigue consonante o viceversa): /// " + usuario + " /// Cantidad de Caracteres: " + std::to_string(usuario.length());
            cout << respuesta << endl;
            Enviar(respuesta);
        }
    }

    bool longitudPasswordCorrecta(int cantidad_caracteres)
    {
        bool res = false;
        if(cantidad_caracteres < 8 ||  cantidad_caracteres > 49)
        {
            string respuesta = "Error desde el Servidor: La contraseña debe tener un mínimo de 8 caracteres y un máximo de 49.";
            Enviar(respuesta);
        } else {
            res = true;
        }
        return res;
    }

    void generarPassword(string mensaje)
    {
        std::string operacion = mensaje.substr(4, mensaje.length());
        if(operacion.length() > 6)
        {
            operacion = operacion.substr(4, 6);
        }
        int cantidad_caracteres = atoi(operacion.c_str());
        if(longitudPasswordCorrecta(cantidad_caracteres))
        {
            std::string password;
            for(int i=0 ; i<cantidad_caracteres;i++)
            {
                password += abecedario[generateRandomNumber(62)];
            }
            string respuesta = "Password Random Alfanumerico (mayus-minus) Generado: /// " + password + " /// Cantidad de Caracteres: " + std::to_string(password.length());
            cout << respuesta << endl;
            Enviar(respuesta);
        }
    }

    void Enviar(string mensaje)
    {
        send(this->clientSocket, mensaje.c_str(), mensaje.size() + 1, 0);
        cout << "Respuesta enviada!" << endl;
    }

    void CerrarSocket()
    {
        cout << "**CLIENTE DESCONECTADO**" << endl;
        close(this->clientSocket);
    }

    int generateRandomNumber(int limit)
    {
        int numero;
        srand((unsigned) time(NULL) * rand());
        numero = rand();
        if(limit > 0)
        {
            numero = numero % limit;
        }
        return numero;
    }

};

int main()
{
    Servidor *Server = new Servidor();
    while(Server->Conectar())
    {
        while(Server->Recibir())
        {
            Server->Recibir();
        }
    }
}
