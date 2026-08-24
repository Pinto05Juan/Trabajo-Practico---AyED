#include <iostream>
#include <cstdio>

using namespace std;

struct Comanda
{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

void generarNombreDelArchivo(char nombreArchivo[]);
void leerComandas(const char nombreArchivo[]);

int main()
{
    char nombreArchivo[26];

    generarNombreDelArchivo(nombreArchivo);

    leerComandas(nombreArchivo);

    return 0;
}

void leerComandas(const char nombreArchivo[])
{
    FILE* archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL)
    {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    Comanda comanda;

    while (fread(&comanda, sizeof(Comanda), 1, archivo) == 1)
    {
        cout << "Mozo: " << comanda.idMozo << endl;
        cout << "Producto: " << comanda.codigoProducto << endl;
        cout << "Cantidad: " << comanda.cantidad << endl;
        cout << "Comision: $" << comanda.comision << endl;
        cout << endl;
    }

    fclose(archivo);
}

void generarNombreDelArchivo(char nombreArchivo[])
{
    int semana;
    int mes;

    cout << "Ingrese numero de semana: ";
    cin >> semana;

    cout << "Ingrese mes: ";
    cin >> mes;

    nombreArchivo[0] = 'c';
    nombreArchivo[1] = 'o';
    nombreArchivo[2] = 'm';
    nombreArchivo[3] = 'a';
    nombreArchivo[4] = 'n';
    nombreArchivo[5] = 'd';
    nombreArchivo[6] = 'a';
    nombreArchivo[7] = 's';
    nombreArchivo[8] = '_';

    nombreArchivo[9] = 's';
    nombreArchivo[10] = 'e';
    nombreArchivo[11] = 'm';
    nombreArchivo[12] = 'a';
    nombreArchivo[13] = 'n';
    nombreArchivo[14] = 'a';
    nombreArchivo[15] = '_';

    nombreArchivo[16] = 's';
    nombreArchivo[17] = '0' + semana;

    nombreArchivo[18] = '-';

    nombreArchivo[19] = '0' + mes / 10;
    nombreArchivo[20] = '0' + mes % 10;

    nombreArchivo[21] = '.';
    nombreArchivo[22] = 'd';
    nombreArchivo[23] = 'a';
    nombreArchivo[24] = 't';
    nombreArchivo[25] = '\0';
}




