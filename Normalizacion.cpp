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
	for (int i=0; i<cantMozos; i++){
		if (strcmp(mozosArr[i].nombre, nombre)==0){
			return mozosArr[i].idMozo;
		}
	}
		return -1;
		}

void ArmarNombreArchivoDia (char fecha[], char nombreArchivo[]){ //funcion auxiliar para armar el nombre del archivo a partir de la fecha
	sprintf(nombreArchivo, "comandas_%s.dat", fecha); 
}

void registrarFecha(char fechas[][11], int& cantFechas, char fecha[])
{
    for(int i = 0; i < cantFechas; i++)
    {
        if(strcmp(fechas[i], fecha) == 0)
            return; // ya la tenía registrada
    }
    strcpy(fechas[cantFechas], fecha);
    cantFechas++;
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

void OrdenarArchivoPorMozo (char nombreDeArchivo[]){
	Comanda comandasDias [200]; //alcanza para todas las ventas de un solo dia
	int cantComandas=0;

	FILE* fDia=fopen(nombreDeArchivo, "rb"); //leo todo el archivo del dia y lo cargo en la memoria
	while (fread(&comandasDias[cantComandas], sizeof (Comanda), 1, fDia)==1){
		cantComandas++; //cuento cuantas comandas tiene el dia
	}
	fclose (fDia);

	for (int i=0; i<cantComandas-1; i++){ //ordeno por id mozo conn el metodo de burbuja
		for (int j=0; j<cantComandas-1-i; j++){ //achico con -i porque en cada pasada ya queda un elemento bien ubicado al final
			if (comandasDias[j].idMozo> comandasDias[j+1].idMozo){
				Comanda aux=comandasDias[j];
				comandasDias[j]=comandasDias[j+1];
				comandasDias[j+1]=aux;
			}
		}
	}

	FILE* fDiaEscritura = fopen  (nombreDeArchivo, "wb"); //reescribo el archivo ya ordenado
	fwrite (comandasDias, sizeof (Comanda), cantComandas, fDiaEscritura);
	fclose (fDiaEscritura);
}


void ActualizarStock (int codigos[], int cantVendida[], int cantCodigos, char archivoInventario[]){ //resta del inventario lo que se vendio de cada producto

FILE* fInv=fopen (archivoInventario, "rb+"); //uso rb+ en lugar de wb para leer y escribir sin truncar el archivo
if(fInv==NULL){
	cout<<"Error al abrir inventario"<<endl;
	return;
}
Producto p;
while (fread(&p, sizeof(Producto), 1, fInv)==1){
	for (int i=0; i<cantCodigos; i++){ //busco si el producto tiene ventas acumuladas
		if(codigos[i]==p.codigo){
			p.stockActual-=cantVendida[i];
			if (p.stockActual<0){ //munca deja stock negativo
				p.stockActual=0;
			}
			fseek (fInv, -(long)sizeof(Producto), SEEK_CUR); // retrocedo al inicio del registro
			fwrite (&p, sizeof(Producto), 1, fInv); //sobreescribo solo ese produco
			fflush (fInv);
			fseek(fInv, 0, SEEK_CUR); // reseteo el stream antes del proximo read
			break;
			}
		}
	}
	fclose (fInv);
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
	FILE* fMozosLectura=fopen("mozos.dat", "rb");
	while (fread(&mozosArr[cantMozos], sizeof(Mozo), 1, fMozosLectura)==1)
		cantMozos++;
	fclose (fMozosLectura);

	FILE* comHis2=fopen("comandas_historicas.dat", "rb");
	 
	int codigosVendidos[100];
	int cantVendida[100];
	int cantCodigos=0;

	char fechasVistas [50][11];
	int cantFechas=0;

	ComandaHistorica c2;
	while (fread(&c2, sizeof(ComandaHistorica), 1, comHis2)==1){
		int idMozoActual= buscarIdMozo(mozosArr, cantMozos, c2.nombreMozo);

		//armo el registro que va a la planilla del dia usando el idmozo en lugar del nombre
		Comanda nuevaComanda;
		nuevaComanda.idMozo=idMozoActual;
		nuevaComanda.cantidad=c2.cantidad;
		nuevaComanda.comision=c2.comision;
		nuevaComanda.codigoProducto=c2.codigoProducto;

		char nombreArchivoDia[30];
		ArmarNombreArchivoDia(c2.fecha, nombreArchivoDia);

		FILE* fDia=fopen(nombreArchivoDia, "ab"); //uso ab porque si el archivo del dia no existe, lo crea, y si ya existe le agrega al final
		fwrite(&nuevaComanda, sizeof(Comanda), 1, fDia);
		fclose(fDia);

		registrarFecha(fechasVistas, cantFechas, c2.fecha); //guardo la fecha en las fechas vistas

		acumularVenta(codigosVendidos, cantVendida, cantCodigos, c2.codigoProducto, c2.cantidad); //acumulo cuanto se vendio de cada producto para despues actualizar el stock
	}
	fclose (comHis2);

	for (int i=0; i<cantFechas; i++){ //recorro cada fecha que se vio y ordeno su archivo correspondiente
		char nombreArchivoDia[30];
		ArmarNombreArchivoDia(fechasVistas[i], nombreArchivoDia);
		OrdenarArchivoPorMozo(nombreArchivoDia);
	}


	ActualizarStock (codigosVendidos, cantVendida, cantCodigos, "inventario.dat"); //resta del stock lo acumulado por producto sin reescribir el inventario
}
