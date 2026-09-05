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
		int i = 0;
		for(i=0; clave[i] != '\0'; i++)
		{
			clave[i] += k;
		}
	}

void procesarMozo(FILE* fMozos, ComandaHistorica c, int& idMozo)
	{
	Mozo mozoAux;
	bool mozoEncontrado = false;
	fseek(fMozos, 0, SEEK_SET);
	while(fread(&mozoAux, sizeof(Mozo), 1, fMozos) == 1)
	{
		if (strcmp(mozoAux.nombre, c.nombreMozo) == 0) //mismo nombre
		{
			mozoEncontrado = true;
			mozoAux.totalComision += c.comision;
				
			fseek(fMozos, -(long)sizeof(Mozo), SEEK_CUR); //retrocedo puntero 
			fwrite(&mozoAux, sizeof(Mozo), 1, fMozos); //Actualizo archivo de mozos
			break; //salgo para continuar con el siguiente 
		}
	}
		
	if(!mozoEncontrado)
	{
		Mozo newMozo;
		newMozo.idMozo = idMozo; //Como es un arhivo nuevo, la primera vez que pase por este if sera el primer id y despues el segundo y asi sucesivamente
		strcpy(newMozo.nombre, c.nombreMozo);
		newMozo.totalComision = c.comision; //la comanda arranca en el valor de la primera comanda
	
		char claveM[20];
		sprintf(claveM, "%d", newMozo.idMozo); //copio el id del mozo en el char de la clave, como es entero uso d
		Clave(claveM, k);
		strcpy(newMozo.password, claveM); //cargo la clave en el nuevo mozo
		
		fseek(fMozos, 0, SEEK_END); //Al final para caegar el nuevo mozo en orden
		fwrite(&newMozo, sizeof(Mozo), 1, fMozos);
		idMozo++;
		}
	}


int buscarIdMozo (Mozo mozosArr[], int cantMozos, char nombre[]){
	for (int i=0; i<cantMozos; i++);
		if (strcmp(mozosArr[i].nombre, nombre)==0)
			return mozosArr[i].idMozo;
		return -1;
		}

void ArmarNombreArchivoDia (char fecha[], char nombreArchivo[]){ //funcion auxiliar para armar el nombre del archivo a partir de la fecha
	sprintf(nombreArchivo, "comandas_%s.dat", fecha);
}

void acumularVenta (int codigos[], int cantidades[], int &cantCodigos, int codigoProducto, int cantidadVendida){ //funcion auxiliar para acumular la cantidad vendida por producto
	for (int i=0; i<cantCodigos; i++){
		if(codigos[i]==codigoProducto){
			cantidades[i]+=cantidadVendida;
			return;
		}
	}
	//si no lo encontro, es un producto nuevo en el acumulado
	 codigos[cantCodigos] = codigoProducto;
	 cantidades[cantCodigos] = cantidadVendida;
	 cantCodigos++;
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

	Mozo mozosArr[100];
	int cantMozos=0;
	File* fMozosLectura=fopen("mozos.dat", "rb");
	while (fread(&mozosArr[cantMozos], sizeof(Mozo), 1, fMozosLectura)==1)
		cantMozos++;
	fclose (fMozosLectura);
}
