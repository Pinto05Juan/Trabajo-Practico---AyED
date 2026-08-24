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








int main()
{
	
}
