#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct ComandaHistorica 
{ 
char fecha[11];        
// "DD-MM-AAAA" 
char nombreMozo[50];   // el nombre completo, repetido en cada venta 
int codigoProducto;   
int cantidad;
float comision; 
};
 
struct Producto { 
    int   codigo;   
	char descripcion[50];   
	float precio;   
	int stockActual; 
};  

struct Mozo
{   
int idMozo; 
char nombre[50]; 
char password[20]; 
float totalComision; 
};

struct Comanda 
{  
int idMozo; 
int codigoProducto; 
int cantidad; 
float comision; 
}; 

const float TASA_COMISION = 0.10f;
const int k = 7;

void Clave(char* clave, int k)
	{
	
	}

void procesarMozo(FILE* fMozos, ComandaHistorica c, int& idMozo)
	{
		
	}


int main()
{
	FILE* comHis = fopen("comandas_historicas.dat", "rb");
	if(comHis == NULL)
		{
		cout<<"Error de apertura"<<endl;
		return 0;
		}
	FILE* fMozos = fopen("mozos.dat", "wb+"); //creo el archivo de mozos para leer y escribir 
	if(fMozos == NULL)
		{
		cout<<"Error de creacion de archivo"<<endl;
		fclose(comHis); //cierro el archivo de comandas porque salio todo mal
		return 0;
		}
	
	int idMozo = 100;   //para asignar el id de 100 en adelante sumando +1
	ComandaHistorica c;
	
	while(fread(&c, sizeof(ComandaHistorica), 1, comHis) == 1)
	{
		procesarMozo(fMozos, c, idMozo);
	}
	fclose(comHis);
	fclose(fMozos);	
}
