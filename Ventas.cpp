#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

//STRUCTS
struct Producto {int codigo; char descripcion[50]; float precio; int stockActual;};

struct Mozo { int idMozo; char nombre[50]; char password[20]; float totalComision; };
struct Comanda { int idMozo; int codigoProducto; int cantidad; float comision; };

//CONST
const float TASA_COMISION = 0.10f; // la comision de cada venta es el 10% de lo vendido
const int K = 7; //codificacion de clave
const int ID_INICIAL = 100;
const long ERROR_ABRIENDO_INVENTARIO = -10;

//PROTOTIPOS DE LAS FUNCIONES
void ingreso_fecha(int &dia, int &mes, int &anio);
bool login_mozo(int &id_mozo, char clave[]);
void codificar_clave(char clave[]);
bool comparar_claves(char clave_ingresada[], char clave_guardada[]);

long busquedaBinaria(const char* nombre, int codigo, Producto &p);
bool actualizar_inventario(int &codigo_producto, int &cantidad, float &comision);

bool actualizar_comision(int id_mozo, float comision);

void generar_nombre_del_archivo(char nombre_archivo[]);
void generar_planilla_del_dia(char nombre_del_archivo[]);

void ordenar(char nombre_del_archivo[]);

//FUNCIONES AUXILIARES (NO FORMAN PARTE DEL CODIGO DEFINITIVO)

void leer_planilla_del_dia(char nombre_archivo[]);
//void mostrar_inventario();


//MAIN
int main(){
    
    char nombre_del_archivo[24];
    
    generar_nombre_del_archivo(nombre_del_archivo);
    generar_planilla_del_dia(nombre_del_archivo);

    //funciones auxiliares para verificar el funcionamiento del programa
    leer_planilla_del_dia(nombre_del_archivo);
    //mostrar_inventario();
}

//DEFINICION DE FUNCIONES
void ingreso_fecha(int &dia, int &mes, int &anio){
    //Medida de control: verifica que la fecha exista para que Alberto no pueda cometer errores a la hora de cargar la fecha
    bool valido=false;
    while(!valido){
        cout << "Ingrese dia (1-31): ";
        cin >> dia;

        if(dia <1 || dia > 31){
            cout << "Dia invalido. Reintentar" << endl << endl;
        }
        else
            valido=true;
    }
    
    valido=false;
    while(!valido){
        cout << endl << "Ingrese mes (1-12): ";
        cin >> mes;

        if(mes <1 || mes > 12 
            || (dia>29 && mes==2)   //febrero tiene como maximo 29 dias
            || (dia == 31  && (mes==4 || mes== 6 || mes == 9 || mes == 11))){   //abril, junio, septiembre y noviembre tienen 30 dias
            cout << "Mes invalido. Reintentar" << endl;
        }

        else
            valido=true;
    }

    valido=false;
    while(!valido){
        cout << endl << "Ingrese anio (2025 en adelante): ";
        cin >> anio;

        if(anio < 2025){
            cout << "Anio invalido. Reintentar" << endl;
        }

        /*Un año es bisiesto si cumple alguna de estas dos condiciones:
        a) es divisible por 4 y no es divisible por 100
        b) es divisible por 400
        */
        else if((dia == 29 && mes == 2)){
            if(!((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0))){
                cout<<"El anio ingresado no es bisiesto. Reintentar"<<endl;
            }
            else{
                valido=true;
            }
        }   

        else
            valido=true;
    }
}

bool login_mozo(int &id_mozo, char clave[]){     //DEVUELVE 1 SI SE INGRESARON DATOS CORRECTOS O 0 SI OCURRIO UN ERROR CON EL ARCHIVO
    FILE* arch_mozos=fopen("Mozos.dat","rb");

    if(arch_mozos==NULL){
        cout << "No se pudo abrir el archivo 'Mozos.dat'" << endl;
        return 0;
    }
    
    Mozo m;
    bool encontrado=false;

    do{
        cout << endl << "Ingrese ID del mozo: ";
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
            
            if(leido==1 && m.idMozo==id_mozo && comparar_claves(m.password,clave)==1){     //ID y clave ingresada son correctos
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

bool comparar_claves(char clave_ingresada[], char clave_guardada[]){
    int i = 0;

    while(clave_ingresada[i] != '\0' && clave_guardada[i] != '\0'){
        if(clave_ingresada[i] != clave_guardada[i]){
            return false;
        }
        i++;
    }

    if(clave_ingresada[i] != '\0' || clave_guardada[i] != '\0'){    //revisa si una clave es mas corta que la otra
        return false;
    }

    return true;    //las claves son iguales
}

bool actualizar_inventario(int &codigo_producto, int &cantidad, float &comision){   
    Producto prod;
    long posicion_prod = -1;
    bool error_stock = true;

    do{
        cout << "Ingrese codigo de producto: ";
        cin >> codigo_producto;
        
        //busco el producto en el archivo
        posicion_prod=busquedaBinaria("inventario.dat", codigo_producto, prod);

        if(posicion_prod==ERROR_ABRIENDO_INVENTARIO){
            return false;
        }
        else if(posicion_prod==-1){
            cout << "Producto no encontrado. Reintentar." << endl;
        }
    } while(posicion_prod==-1);

    do{
        cout << "Ingrese cantidad: ";
        cin >> cantidad;

        if(cantidad<=0){
            cout << "La cantidad ingresada no es valida. Reintentar." << endl;
        }

        else if(prod.stockActual-cantidad < 0){
            cout << "No hay suficiente stock. La cantidad ingresada es incorrecta. Reintentar." << endl;
        }
        else{
            error_stock=false;
        }
    } while (error_stock == true);

    //abro el archivo en modo rb+ para sobreescribir
    FILE* arch_inventario=fopen("inventario.dat","rb+");

    if(arch_inventario==NULL){
        cout << "No se pudo abrir el archivo 'Inventario.dat'. Intentelo mas tarde." << endl;
        return false;
    }

    prod.stockActual-=cantidad;

    fseek(arch_inventario, posicion_prod*sizeof(Producto),SEEK_SET);   
    fwrite(&prod, sizeof(Producto),1,arch_inventario);
    
    fclose(arch_inventario);

    comision= prod.precio * cantidad * TASA_COMISION;
    
    return true;
}

long busquedaBinaria(const char* nombre, int codigo, Producto &p) {
    FILE* f = fopen(nombre, "rb");
    if (f == NULL){
        cout << "No se pudo abrir 'inventario.dat'"<<endl;
        return ERROR_ABRIENDO_INVENTARIO;
    } 

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

bool actualizar_comision(int id_mozo, float comision){
    FILE* arch_mozos=fopen("Mozos.dat","rb+");

    if(arch_mozos==NULL){
        cout << "No se pudo abrir el archivo 'Mozos.dat'. Intentelo mas tarde";
        return false;
    }
    
    Mozo m;

    int pos=id_mozo-ID_INICIAL;

    fseek(arch_mozos, pos*sizeof(m), SEEK_SET);
    int leido=fread(&m, sizeof(Mozo),1,arch_mozos)==1;
    
    if(leido==0 || m.idMozo != id_mozo){
        cout << "Ha ocurrido un error. Reintentelo mas tarde"<<endl;
        fclose(arch_mozos);
        return false;
    }

    m.totalComision+=comision;
    fseek(arch_mozos, pos*sizeof(m), SEEK_SET);
    fwrite(&m,sizeof(Mozo),1,arch_mozos);

    fclose(arch_mozos);
    
    return true;
}

void generar_nombre_del_archivo(char nombre_archivo[]){
    int dia, mes, anio;
    ingreso_fecha(dia,mes,anio);
    
    nombre_archivo[0] = 'c';
    nombre_archivo[1] = 'o';
    nombre_archivo[2] = 'm';
    nombre_archivo[3] = 'a';
    nombre_archivo[4] = 'n';
    nombre_archivo[5] = 'd';
    nombre_archivo[6] = 'a';
    nombre_archivo[7] = 's';
    nombre_archivo[8] = '_';

    nombre_archivo[9] = '0' + dia / 10;
    nombre_archivo[10] = '0' + dia % 10;

    nombre_archivo[11] = '-';

    nombre_archivo[12] = '0' + mes / 10;
    nombre_archivo[13] = '0' + mes % 10;

    nombre_archivo[14] = '-';

    nombre_archivo[15] = '0' + anio / 1000;
    nombre_archivo[16] = '0' + (anio / 100) % 10;
    nombre_archivo[17] = '0' + (anio / 10) % 10;
    nombre_archivo[18] = '0' + anio % 10;

    nombre_archivo[19] = '.';
    nombre_archivo[20] = 'd';
    nombre_archivo[21] = 'a';
    nombre_archivo[22] = 't';
    nombre_archivo[23] = '\0';
}

void generar_planilla_del_dia(char nombre_del_archivo[]){
    FILE* planilla = fopen(nombre_del_archivo,"ab+");

    if(planilla==NULL){
        cout << "Error al crear planilla del dia"<<endl;
        return;
    }

    cout << endl <<"Bienvenido a la planilla del dia " << nombre_del_archivo[9] << nombre_del_archivo[10] 
    << "/" << nombre_del_archivo[12] << nombre_del_archivo[13] 
    << "/" << nombre_del_archivo[15]  << nombre_del_archivo[16] << nombre_del_archivo[17] << nombre_del_archivo[18] 
    << endl << "Cargue todas las ventas correspondientes: " << endl << endl;
        
    int seguir=1;    

    while(seguir==1){
        int id_mozo; char clave[20];
        bool login=login_mozo(id_mozo, clave);

        if(login==false){
            cout << "Error con el login. Reintentelo mas tarde." << endl;
            fclose(planilla);
            return;
        }
        
        else{
            int codigo_producto, cantidad;
            float comision;

            if(actualizar_inventario(codigo_producto,cantidad,comision)==false){
                cout << "Ha ocurrido un error durante la actualizacion del inventario. Reintentelo mas tarde.";
                fclose(planilla);
                return;
            }

            if(actualizar_comision(id_mozo, comision)==false){
                cout << "Ha ocurrido un error durante la actualizacion de la comision. Reintentelo mas tarde.";
                fclose(planilla);
                return;
            }
            
            Comanda venta;
            venta.idMozo=id_mozo;
            venta.codigoProducto=codigo_producto;
            venta.cantidad=cantidad;
            venta.comision=comision;

            fwrite(&venta, sizeof(Comanda),1,planilla);

            cout << endl << "Desea cargar otra venta? (0: No | 1: Si): ";
            cin >> seguir;

            while(seguir!=0 && seguir!=1){
                cout << "Error. Ingrese un numero valido (0: No | 1: Si): ";
                cin >> seguir;
            }
        }       
    }
    fclose(planilla);
    ordenar(nombre_del_archivo);
    cout << endl << "Planilla generada/actualizada con exito!";
}

void ordenar(char nombre_del_archivo[]){
    FILE* planilla = fopen(nombre_del_archivo,"rb+");

    if(planilla==NULL){
        cout << "Error al abrir planilla del dia para ordenarla"<<endl;
        return;
    }

    // Averiguamos cuántas comandas hay
    fseek(planilla, 0, SEEK_END);
    long cantidad = ftell(planilla) / sizeof(Comanda);

    Comanda menor;
    Comanda actual;
    Comanda aux;

    for (long i = 0; i < cantidad - 1; i++) {
        
        fseek(planilla, i * sizeof(Comanda), SEEK_SET); 
        fread(&aux, sizeof(Comanda), 1, planilla);  // almacena la venta original que esta en la posicion i
        
        menor = aux;    //supone que esa es la de menor id_mozo
        long posicionMenor = i;

        // recorre las demas posiciones buscando una venta con menor id_mozo
        for (long j = i + 1; j < cantidad; j++) {
            fseek(planilla, j * sizeof(Comanda), SEEK_SET);
            fread(&actual, sizeof(Comanda), 1, planilla);

            if (actual.idMozo < menor.idMozo) {     //si encuentra una, actualiza "menor" y almacena esa posicion
                menor = actual;
                posicionMenor = j;
            }
        }

        //realizar el intercambio si se encontro una venta con menor id_mozo
        if (posicionMenor != i) {       
            fseek(planilla, i * sizeof(Comanda), SEEK_SET);
            fwrite(&menor, sizeof(Comanda), 1, planilla);   //escribe la venta de menor id_mozo en la posicion i

            fseek(planilla, posicionMenor * sizeof(Comanda), SEEK_SET);
            fwrite(&aux, sizeof(Comanda), 1, planilla);     //escribe la venta original de la posicion i en donde estaba la venta de menor id_mozo
        }
    }

    fclose(planilla);
}


//FUNCIONES AUXILIARES (NO FORMAN PARTE DEL PROGRAMA DEFINITIVO, SOLO SE USAN PARA PROBAR EL FUNCIONAMIENTO)

void leer_planilla_del_dia(char nombre_archivo[]){      //funcion auxiliar para imprimir comandas_dd-mm-aaaa y verificar que este ordenado
    FILE* arch=fopen(nombre_archivo,"rb");
    if(arch==NULL){
        cout <<"error al leer la planilla del dia"<<endl;
        return;
    }
    Comanda comanda;
    while(fread(&comanda,sizeof(Comanda),1,arch)==1){
        cout << "ID: " << comanda.idMozo <<endl;
        cout << "Codigo producto: " << comanda.codigoProducto<<endl;
        cout << "Cantidad: " << comanda.cantidad<<endl;
        cout << "Comision: $" << comanda.comision<<endl<<endl;
    }
    fclose(arch);
}

/*
void mostrar_inventario(){
    FILE* arch=fopen("inventario.dat","rb");
    if(arch==NULL){
        cout <<"error al leer el inventario"<<endl;
        return;
    }

    Producto p;
    cout<< endl;
    while(fread(&p, sizeof(Producto),1,arch)){
        cout<< endl<< "Codigo: " <<p.codigo <<"    Descripcion: "<<p.descripcion << "  Precio: $"<<p.precio<<"  Stock: " <<p.stockActual;
    }

    fclose(arch);
}
*/