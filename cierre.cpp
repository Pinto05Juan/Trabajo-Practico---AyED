#include <iostream>

using namespace std;


struct Comanda { int idMozo; int codigoProducto; int cantidad; float comision; };

void generarNombreDelArchivoSemanal(char nombre_archivo[]);
void ingresoFecha(int &mes, int& anio);
int getDiasDelMes(int mes, int anio);
void generarPlanillaSemanal();
void apareoDePlanillas();

int main() {
    int mes, int anio;
    int dia = 1; 


    char fecha [26];

    generarNombreDelArchivoSemanal(fecha);


    return 0;
}

void generarPlanillaSemanal() {
    
}

void generarNombreDelDia(char nombreArchivo[], int dia, int mes, int anio) {
    nombreArchivo[0] = 'c';
    nombreArchivo[1] = 'o';
    nombreArchivo[2] = 'm';
    nombreArchivo[3] = 'a';
    nombreArchivo[4] = 'n';
    nombreArchivo[5] = 'd';
    nombreArchivo[6] = 'a';
    nombreArchivo[7] = 's';
    nombreArchivo[8] = '_';

    nombreArchivo[9] = '0' + dia / 10;
    nombreArchivo[10] = '0' + dia % 10;

    nombreArchivo[11] = '-';

    nombreArchivo[12] = '0' + mes / 10;
    nombreArchivo[13] = '0' + mes % 10;

    nombreArchivo[14] = '-';

    nombreArchivo[15] = '0' + anio / 1000;
    nombreArchivo[16] = '0' + (anio / 100) % 10;
    nombreArchivo[17] = '0' + (anio / 10) % 10;
    nombreArchivo[18] = '0' + anio % 10;

    nombreArchivo[19] = '.';
    nombreArchivo[20] = 'd';
    nombreArchivo[21] = 'a';
    nombreArchivo[22] = 't';
    nombreArchivo[23] = '\0';
}

int getDiasDelMes(int mes, int anio) {
    int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)) {
        return 29;
    }

    return dias[mes - 1];
}

void apareoDePlanillas(const char* nomA, const char* nomB, const char* nomC) {
    FILE* planillaA = fopen(nomA, "rb");
    FILE* planillaB = fopen(nomB, "rb");
    FILE* nuevaPlanilla = fopen(nomC, "wb");

    if(planillaA == NULL || planillaB == NULL || nuevaPlanilla == NULL){
        cout << "Error al abrir los archivos" << endl;
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

void generarNombreDelArchivoSemanal(char nombre_archivo[], int numeroDeSemana){
    int mes, anio;
    ingresoFecha(mes,anio);
    
    nombre_archivo[0] = 'c';
    nombre_archivo[1] = 'o';
    nombre_archivo[2] = 'm';
    nombre_archivo[3] = 'a';
    nombre_archivo[4] = 'n';
    nombre_archivo[5] = 'd';
    nombre_archivo[6] = 'a';
    nombre_archivo[7] = 's';
    nombre_archivo[8] = '_';

    nombre_archivo[9] = 's';
    nombre_archivo[10] = 'e';
    nombre_archivo[11] = 'm';
    nombre_archivo[12] = 'a';
    nombre_archivo[13] = 'n';
    nombre_archivo[14] = 'a';
    nombre_archivo[15] = '_';

    nombre_archivo[16] = 's';
    nombre_archivo[17] = '0' + numeroDeSemana; // ACA VA EL NUMERO DE LA SEMANA
    nombre_archivo[18] = '-';

    nombre_archivo[19] = '0' + mes / 10;;
    nombre_archivo[20] = '0' + mes % 10;
    nombre_archivo[21] = '.';
    nombre_archivo[22] = 'd';
    nombre_archivo[23] = 'a';
    nombre_archivo[24] = 't';
    nombre_archivo[25] = '\0';
}