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

long busquedaBinaria(const char* nombre, int codigo, Producto &p);
float actualizar_inventario(int &codigo_producto, int &cantidad);

void actualizar_comision(int id_mozo, float comision);

void leer(); //funcion para probar, no forma parte del codigo definitivo

void cargar();
void mostrar_mozos();   //funcion auxiliar, no forma parte del programa definitivo

//MAIN
int main(){
    int dia, mes, anio;
    ingreso_fecha(dia,mes,anio);
    
    //cargar();
    int id_mozo; char clave[20];
    int login=login_mozo(id_mozo, clave);

    if(login==-1){
        cout << "Error. Reintentelo mas tarde." << endl;
        return 0;
    }
    
    int codigo_producto, cantidad;
    float comision= actualizar_inventario(codigo_producto,cantidad);
    
    actualizar_comision(id_mozo, comision);
    mostrar_mozos();    //funcion para verificar si se actualiza la comision correctamente
    
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

        if(id_mozo < ID_INICIAL){
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

float actualizar_inventario(int &codigo_producto, int &cantidad){   
    Producto prod;
    long posicion_prod = -1;
    bool error_stock = true;

    do{
        cout << "Ingrese codigo de producto: ";
        cin >> codigo_producto;
        
        //busco el producto en el archivo
        posicion_prod=busquedaBinaria("inventario2.dat", codigo_producto, prod);

        if(posicion_prod==-1){
            cout << "Producto no encontrado. Reintentar." << endl;
        }
    } while(posicion_prod==-1);

    do{
        cout << "Ingrese cantidad: ";
            cin >> cantidad;

            if(prod.stockActual-cantidad < 0){
                cout << "No hay suficiente stock. La cantidad ingresada es incorrecta. Reintentar." << endl;
            }
            else{
                error_stock=false;
            }
    } while (error_stock == true);

    //abro el archivo en modo rb+ para sobreescribir
    FILE* arch_inventario=fopen("inventario2.dat","rb+");

    if(arch_inventario==NULL){
        cout << "No se pudo abrir el archivo 'Inventario2.dat'. Intentelo mas tarde." << endl;
        return -1;
    }

    prod.stockActual-=cantidad;
    fseek(arch_inventario, posicion_prod*sizeof(Producto),SEEK_SET);
    
    /*
    int leido=fread(&prod, sizeof(Producto),1,arch_inventario);

    if(leido==0){
        cout << "Ha ocurrido un error. Reintentelo mas tarde"<<endl;
        fclose(arch_inventario);
        return -1;
    }
    */
    
    //fseek(arch_inventario, posicion_prod*sizeof(Producto),SEEK_SET);    //POSICIONARSE NUEVAMENTE EN EL PRODUCTO PARA SOBREESCRIBIR LOS DATOS
    fwrite(&prod, sizeof(Producto),1,arch_inventario);
    
    fclose(arch_inventario);

    float comision= prod.precio * cantidad * TASA_COMISION;
    return comision;
}

long busquedaBinaria(const char* nombre, int codigo, Producto &p) {
    FILE* f = fopen(nombre, "rb");
    if (f == NULL) return -1;

    fseek(f, 0, SEEK_END);
    long n = ftell(f) / sizeof(Producto); // cantidad de registros
    long pri = 0, ult = n - 1, pos = -1;

    while (pri <= ult && pos == -1) {
        long med = (pri + ult) / 2;
        fseek(f, med * sizeof(Producto), SEEK_SET);
        fread(&p, sizeof(Producto), 1, f);
        if (p.codigo == codigo) pos = med;
        else if (codigo > p.codigo) pri = med + 1;
        else ult = med - 1;
    }
    
    fclose(f);
    return pos;
}

void actualizar_comision(int id_mozo, float comision){
    FILE* arch_mozos=fopen("Mozos.dat","rb+");

    if(arch_mozos==NULL){
        cout << "No se pudo abrir el archivo 'Mozos.dat'. Intentelo mas tarde";
        return;
    }
    
    Mozo m;

    int pos=id_mozo-ID_INICIAL;

    fseek(arch_mozos, pos*sizeof(m), SEEK_SET);
    int leido=fread(&m, sizeof(Mozo),1,arch_mozos)==1;
    
    if(leido==0){
        cout << "Ha ocurrido un error. Reintentelo mas tarde"<<endl;
        fclose(arch_mozos);
        return;
    }

    m.totalComision+=comision;
    fseek(arch_mozos, pos*sizeof(m), SEEK_SET);
    fwrite(&m,sizeof(Mozo),1,arch_mozos);

    fclose(arch_mozos);
    
    return;
}

//NO FORMA PARTE DEL PROGRAMA DEFINITIVO
void mostrar_mozos(){   //funcion auxiliar para verificar si se actualizo la comision correctamente
    FILE* arch_mozos=fopen("Mozos.dat","rb");
    if(arch_mozos==NULL){
        cout << "no se pudo abrir mozos.dat";
        return;
    }

    Mozo m;
    while(fread(&m,sizeof(Mozo),1,arch_mozos)==1){
        cout<<"id: "<<m.idMozo<<endl;
        cout<<"comision: "<<m.totalComision<<endl<<endl;
    }

    fclose(arch_mozos);
}

void leer() //funcion para verificar el funcionamiento hasta ahora. se creo "inventarios2.dat" para hacer las pruebas y no modificar el archivo original
{
    FILE* arch_inv= fopen("Inventario2.dat","rb");
    if(arch_inv==NULL){
        cout << "No se pudo abrir.";
        return;
    }

    Producto p;
    while(fread(&p,sizeof(Producto),1,arch_inv)==1){
        cout<<"codigo: " << p.codigo << endl;
        cout<<"descrripcion: " << p.descripcion << endl;
        cout<<"precio: " << p.precio << endl;
        cout<<"stock actual: " << p.stockActual << endl << endl;        
    }

    fclose(arch_inv);
}