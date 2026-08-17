#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

//STRUCTS
struct Producto {
    int codigo; char descripcion[50]; float precio; int stockActual;
};

struct Mozo { int idMozo; char nombre[50]; char password[20]; float totalComision; };
struct Comanda { int idMozo; int codigoProducto; int cantidad; float comision; };

//CONST
const float TASA_COMISION = 0.10f; // la comision de cada venta es el 10% de lo vendido
const int K = 7; //codificacion de clave
const int ID_INICIAL = 100;

//PROTOTIPOS DE LAS FUNCIONES
void ingreso_fecha(int &dia, int &mes, int &anio);
int login_mozo(int &id_mozo, char clave[]);
void codificar_clave(char clave[]);
void cargar();

//MAIN
int main(){
    int dia, mes, anio;
    ingreso_fecha(dia,mes,anio);
    cargar();
    int id_mozo; char clave[20];
    int login=login_mozo(id_mozo, clave);

    if(login==-1){
        cout << "Error. Reintentelo mas tarde." << endl;
        return 0;
    }
    else{
        cout << endl << "Login exitoso";
        return 0;
    }

}

//DEFINICION DE FUNCIONES
void ingreso_fecha(int &dia, int &mes, int &anio){
    //Medida de control: verifica que la fecha exista para que Alberto no pueda cometer errores a la hora de cargar la fecha
    bool valido=false;
    while(!valido){
        cout << "Ingrese dia (1-31): ";
        cin >> dia;

        if(dia <1 || dia > 31){
            cout << "Dia invalido. Reintentar" << endl;
        }
        else
            valido=true;
    }
    
    valido=false;
    while(!valido){
        cout << endl << "Ingrese mes (1-12): ";
        cin >> mes;

        if(mes <1 || mes > 12 
            || (dia>29 && mes==2)   //febrero tiene como maximo 28 dias
            || (dia == 31  && (mes==4 || mes== 6 || mes == 9 || mes == 11))){   //abril, junio, septiembre y noviembre tienen 30 dias
            cout << "Mes invalido. Reintentar" << endl;
        }

        else
            valido=true;
    }

    valido=false;
    while(!valido){
        cout << endl << "Ingrese anio: ";
        cin >> anio;

        if(anio < 2025){
            cout << "Anio invalido. Reintentar" << endl;
        }

        else
            valido=true;
    }
}

int login_mozo(int &id_mozo, char clave[]){     //DEVUELVE 1 SI SE INGRESARON DATOS CORRECTOS O -1 SI OCURRIO UN ERROR CON EL ARCHIVO
    FILE* arch_mozos=fopen("Mozos.dat","rb");

    if(arch_mozos==NULL){
        cout << "No se pudo abrir el archivo 'Mozos.dat'" << endl;
        return -1;
    }
    
    Mozo m;
    bool encontrado=false;

    do{
        cout << "Ingrese ID del mozo: ";
        cin >> id_mozo;

        cout << "Ingrese clave: ";
        cin >> clave;
        codificar_clave(clave);

        if(id_mozo < 100){
            cout << endl << "El ID o la clave ingresado son incorrectos. Reintentar" << endl;
        }

        else{
            int pos=id_mozo-ID_INICIAL;     

            fseek(arch_mozos, pos*sizeof(m), SEEK_SET); //PUP, posicionarse donde "deberia" estar el mozo buscado
            int leido=fread(&m, sizeof(Mozo),1,arch_mozos);
            
            if(leido==1 && m.idMozo==id_mozo && strcmp(m.password,clave)==0){     //ID y clave ingresada son correctos
                encontrado=true;
            }

            else {
                cout << endl << "El ID o la clave ingresado son incorrectos. Reintentar" << endl;
            }
        }
    } while(encontrado==false);
    
    fclose(arch_mozos);
    return 1;
}

void codificar_clave(char clave[]) {
    int i = 0;

    while(clave[i] != '\0') {
        clave[i] += K;
        i++;
    }
}

//FUNCION PARA PROBAR EL LOGIN, NO FORMA PARTE DEL PROGRAMA DEFINITIVO
void cargar(){
    FILE* arch_mozos=fopen("Mozos.dat","wb");
    for(int i=0; i<5; i++){
        Mozo m;

        cout<<"ingrese id: ";
        cin >> m.idMozo;
        cout<<"ingrese pass: ";
        cin >> m.password;
        codificar_clave(m.password);
        m.totalComision=0;

        fwrite(&m, sizeof(Mozo),1,arch_mozos);
    }
    fclose(arch_mozos);
}