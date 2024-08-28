#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <sstream>

using namespace std;

bool listoParaEnviar;
bool listoParaTrabajar = true;
int MostrarMenu()
{
    int menu = 0;
    cout << "**MENU: **" <<endl;
    cout << "1 - Generar Usuario" <<endl;
    cout << "2 - Generar Contraseña" <<endl;
    cout << "3 - Cerrar Conexión" <<endl;
    cout << "Ingrese una opción: " <<endl;
    cin>>menu;
    cin.clear();
    cin.ignore(10000, '\n');
    return menu;
}

class Cliente{
public:
    int sock,connectRes;
    Cliente(){ }
    int Conectar()
    {
        int conexion = -1;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            cerr << "No se puede crear el socket. Abortar." << endl;
        }

        //cout << "Ingrese el puerto a Conectarse:"<<endl;
        int port = 5000;
        string ipAddress = "127.0.0.1";
        //cin>>port;

        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

        //conectamos con el socket del servidor
        connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
        if (connectRes == -1)
        {
            cerr << "No se puede conectar el socket. Abortar." << endl;
            conexion = -1;

       }else{
            cout << "/*/*/*Cliente conectado*/*/*/"<<endl;
            conexion = 1;
        }
        return conexion;
    }

    void Inicio()
    {
        listoParaEnviar = true;
        int menu_elegido = MostrarMenu();
        switch(menu_elegido)
        {
            case 1:
                while(listoParaEnviar){
                    this->EnviarUsuario();
                }
                break;
            case 2:
                while(listoParaEnviar){
                    this->EnviarPassword();
                }
                break;
            case 3:
                listoParaEnviar = false;
                this->CerrarConexion();
                break;
        }
    }

    void Enviar(string userInput)
    {
        int sendRes = send(this->sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendRes == -1)
        {
            cerr << "No se pudo conectar con el servidor. \r\n";
        }
    }

    bool analizarTextoIngresado(string mensaje)
    {
        bool res = false;
        if(mensaje != "")
        {
            std::string longitudIngresada = mensaje.substr(0, mensaje.size());
            double d;
            for(int i = 0; i < longitudIngresada.size();i++)
            {
                std::stringstream ss(longitudIngresada);
                if (ss >> d)
                {
                    if (ss.eof())
                    {
                        res = true;
                        break;
                    }
                }
            }
            if(res == false)
            {
                cerr<<"Solo se pueden ingresar números"<<endl;
            }
        }
        return res;
    }

    bool EnviarUsuario()
    {
        string comando = "usua";
        string userInput;
        cin.clear();
        cout << "Ingresar Cantidad de Caracteres de Usuario (5 < x <= 15) > "<<endl;
        getline(cin, userInput);
        bool analizadoOk = analizarTextoIngresado(userInput);
        string respuesta = comando + userInput;
        if(analizadoOk)
        {
            Enviar(respuesta);
            Recibir();
            listoParaEnviar = false;
            this->Inicio();
        }
        return analizadoOk;
    }

    bool EnviarPassword()
    {
        string comando = "cont";
        string userInput;
        cout << "Ingresar Cantidad de Caracteres de Password (8 <= x < 50) > "<<endl;
        getline(cin, userInput);
        bool analizadoOk = analizarTextoIngresado(userInput);
        string mensaje = comando + userInput;
        if(analizadoOk)
        {
            Enviar(mensaje);
            Recibir();
            listoParaEnviar = false;
            this->Inicio();
        }
        return analizadoOk;
    }

    string Recibir()
    {
        char buf[4096];
        memset(buf, 0, 4096);
        int bytesReceived = recv(this->sock, buf, 4096, 0);
        if (bytesReceived == -1)
        {
            cout << "There was an error getting response from server\r\n";
        } else {
             cout << "ServidorDice> " << string(buf, bytesReceived) << "\r\n";
        }
        return string(buf, bytesReceived);
    }

    void CerrarConexion()
    {
        this->Enviar("clos");
        //this->Recibir();
        listoParaTrabajar = false;
    }
};

int main()
{
    Cliente *cliente = new Cliente();
    int conexion = cliente->Conectar();
    if(conexion < 0)
    {
      return -1;
    }
    while(listoParaTrabajar)
    {

        cliente->Inicio();
    }
    return 0;
}
