#include <iostream>
#include <cstdio>
using namespace std;

//STRUCTS
struct Producto {
    int codigo; char descripcion[50]; float precio; int stockActual;
};

struct Mozo { int idMozo; char nombre[50]; char password[20]; float totalComision; };
struct Comanda { int idMozo; int codigoProducto; int cantidad; float comision; };

//CONST
const float TASA_COMISION = 0.10f; // la comision de cada venta es el 10% de lo vendido


//PROTOTIPOS DE LAS FUNCIONES
void ingreso_fecha(int &dia, int &mes, int &anio);


//MAIN
int main(){
    
}

//DEFINICION DE FUNCIONES
void ingreso_fecha(int &dia, int &mes, int &anio){
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

        if(mes <1 || mes > 12 || (dia>29 && mes==2)){
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
