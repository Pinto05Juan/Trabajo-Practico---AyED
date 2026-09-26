#include <iostream>

using namespace std;

struct Comanda { int idMozo; int codigoProducto; int cantidad; float comision; };

void generarNombreDelArchivoSemanal(char nombre_archivo[], int numeroDeSemana, int mes);
void ingresoFecha(int &mes, int& anio);
int getDiasDelMes(int mes, int anio);
void copiarContenido(const char* leer, const char* escribir);
void generarNombreDelDia(char nombreArchivo[], int dia, int mes, int anio);
void apareoDePlanillas(const char* nomA, const char* nomB, const char* nomC);

int main() {
    int mes, anio;
    ingresoFecha(mes, anio);

    int diaDelMes = getDiasDelMes(mes, anio);
    int dia = 1; // Representa el dia del mes
    int numeroDeSemana = 1; //semanas del mes calendario
    int semanasGeneradas = 0; //cantidad de semanas generadas por archivo
    bool existComandasEnElMes = false;
    //se guardan en archivos temporales las comandas para su apareo posterios
    const char* temporal1 = "temporal1.dat";
    const char* temporal2 = "temporal2.dat";

    while (dia <= diaDelMes) {
        bool existDia = false;
        const char* actual = temporal1;
        const char* siguiente = temporal2;
        int contadorDias = 0; // Cuantos dias llevo en la semana

        while(contadorDias < 7 && dia <= diaDelMes) {
            char nombreDia[24];
            generarNombreDelDia(nombreDia, dia, mes, anio);

            FILE* prueba = fopen(nombreDia, "rb");

            if(prueba != NULL) {
                
                if(!existDia) { //Cuando registra por primera vez un dia con venta de esa semana
                    copiarContenido(nombreDia, actual);
                    existDia = true;
                    existComandasEnElMes = true;
                } else {
                    apareoDePlanillas(actual, nombreDia, siguiente);
                    const char* aux = actual; //swap para intercambiar los roles de lectura o escritura
                    actual = siguiente;
                    siguiente = aux;
                }

                fclose(prueba);
            }
            contadorDias++;
            dia++;
        }

        if(existDia) { 
            char nombreSemanal[26];
            generarNombreDelArchivoSemanal(nombreSemanal, numeroDeSemana, mes);
            copiarContenido(actual, nombreSemanal); //actual -> acumulado en la semana
            semanasGeneradas++;
        }
        
        numeroDeSemana++;
    }

    if(existComandasEnElMes) {
        cout << "Se generaron: " << semanasGeneradas << " semanas" << endl;
    } else {
        cout << "No se registraron comandas de esa fecha: [mm/aaaa]" << mes << "/" << anio << endl;
    }

    //Se borran ambos archivos temporales
    remove(temporal1);
    remove(temporal2);

    return 0;
}

void copiarContenido(const char* leer, const char* escribir) {
    FILE* origen = fopen(leer, "rb");
    FILE* destino = fopen(escribir, "wb");
    Comanda c;

    if(origen == NULL || destino == NULL) {
        cout << "Error de apertura en: " << leer << " o " << escribir << endl;
        return;
    }

    while(fread(&c, sizeof(Comanda), 1, origen) == 1) {
        fwrite(&c, sizeof(Comanda), 1, destino);
    }

    fclose(origen);
    fclose(destino);
}

void generarNombreDelDia(char nombreArchivo[], int dia, int mes, int anio) {
    sprintf(nombreArchivo, "comandas_%02d-%02d-%04d.dat", dia, mes, anio);
}

int getDiasDelMes(int mes, int anio) {
    int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)) { //caso año bisiesto
        return 29;
    }

    return dias[mes - 1];
}

void apareoDePlanillas(const char* nomA, const char* nomB, const char* nomC) {
    FILE* planillaA = fopen(nomA, "rb");
    FILE* planillaB = fopen(nomB, "rb");
    FILE* nuevaPlanilla = fopen(nomC, "wb");

    if(planillaA == NULL || planillaB == NULL || nuevaPlanilla == NULL){
        cout << "Error al abrir los archivos: " << nomA << ", " << nomB << " o " << nomC << endl;
        return;
    }
    
    Comanda comandaA, comandaB;

    int lecturaA = fread(&comandaA, sizeof(Comanda), 1, planillaA);
    int lecturaB = fread(&comandaB, sizeof(Comanda), 1, planillaB);

    while (lecturaA == 1 && lecturaB == 1) { 
        if (comandaA.idMozo < comandaB.idMozo) {
            fwrite(&comandaA, sizeof(Comanda), 1, nuevaPlanilla);
            lecturaA = fread(&comandaA, sizeof(Comanda), 1, planillaA);
        } else {
            fwrite(&comandaB, sizeof(Comanda), 1, nuevaPlanilla);
            lecturaB = fread(&comandaB, sizeof(Comanda), 1, planillaB);
        }
    }

    while (lecturaA == 1) { 
        fwrite(&comandaA, sizeof(Comanda), 1, nuevaPlanilla);
        lecturaA = fread(&comandaA, sizeof(Comanda), 1, planillaA);
    }

    while (lecturaB == 1) { 
        fwrite(&comandaB, sizeof(Comanda), 1, nuevaPlanilla);
        lecturaB = fread(&comandaB, sizeof(Comanda), 1, planillaB);
    }

    fclose(planillaA); fclose(planillaB); fclose(nuevaPlanilla);
}

void ingresoFecha(int &mes, int& anio) {
    bool valido = false;
    while(!valido){
        cout << endl << "Ingrese mes (1-12): ";
        cin >> mes;

        cout << "Ingrese el anio: " << endl;
        cin >> anio;

        if(mes < 1 || mes > 12) {
            cout << "Mes invalido. Reintentar" << endl;
        } else {
            valido = true;
        }    
    }
}

void generarNombreDelArchivoSemanal(char nombre_archivo[], int numeroDeSemana, int mes){
    sprintf(nombre_archivo, "comandas_semana_s%d-%02d.dat", numeroDeSemana, mes);
}