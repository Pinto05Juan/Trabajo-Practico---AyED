#include <iostream>

using namespace std;


struct Comanda { int idMozo; int codigoProducto; int cantidad; float comision; };

void generarNombreDelArchivo(char nombre_archivo[]);
void ingresoFecha(int &dia, int &mes);
void generarPlanillaSemanal();
void apareoDePlanillas();

int main() {


    return 0;
}

void generarPlanillaSemanal() {
    
}

void apareoDePlanillas(const char* nomA, const char* nomB, const char* nomC) {
    FILE* planillaA = fopen(nomA, "rb");
    FILE* planillaB = fopen(nomB, "rb");
    FILE* nuevaPlanilla = fopen(nomC, "wb");
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


void ingresoFecha(int &dia, int &mes) {
    bool error = false;
    while(!error){
        cout << endl << "Ingrese mes (1-12): ";
        cin >> mes;

        if(mes < 1 || mes > 12 
            || (dia > 29 && mes == 2)   //febrero tiene como maximo 29 dias
            || (dia == 31  && (mes == 4 || mes == 6 || mes == 9 || mes == 11))) {   //abril, junio, septiembre y noviembre tienen 30 dias
            cout << "Mes invalido. Reintentar" << endl;
        } else {
            error = true;
        }    
    }
}

void generarNombreDelArchivo(char nombre_archivo[]){
    int mes, dia;
    ingresoFecha(dia, mes);
    
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
    nombre_archivo[17] = '0'; // ACA VA EL NUMERO DE LA SEMANA
    nombre_archivo[18] = '-';

    nombre_archivo[19] = '0' + mes / 10;;
    nombre_archivo[20] = '0' + mes % 10;
    nombre_archivo[21] = '.';
    nombre_archivo[22] = 'd';
    nombre_archivo[23] = 'a';
    nombre_archivo[24] = 't';
    nombre_archivo[25] = '\0';
}