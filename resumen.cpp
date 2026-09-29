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

struct Producto
{
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};

void generarNombreDelArchivo(char nombreArchivo[]);
void generarResumen(const char nombreArchivo[]);
float buscarPrecio(int codigoProducto);

int main()
{
    char nombreArchivo[26];

    generarNombreDelArchivo(nombreArchivo);

    generarResumen(nombreArchivo);

    return 0;
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

float buscarPrecio(int codigoProducto)
{
    FILE* archivo = fopen("inventario.dat", "rb");

    if (archivo == NULL)
    {
        return -1;
    }

    Producto producto;

    while (fread(&producto, sizeof(Producto), 1, archivo) == 1)
    {
        if (producto.codigo == codigoProducto)
        {
            fclose(archivo);
            return producto.precio;
        }
    }

    fclose(archivo);

    return -1;
}

void generarResumen(const char nombreArchivo[])
{
    FILE* archivo = fopen(nombreArchivo, "rb");

    if (archivo == NULL)
    {
        cout << "No se pudo abrir el archivo semanal." << endl;
        return;
    }

    Comanda comanda;

    int mozoActual = -1;
    int productosVendidos = 0;
    int totalProductos = 0;

    float comisionMozo = 0;
    float totalFacturado = 0;

    while (fread(&comanda, sizeof(Comanda), 1, archivo) == 1)
    {
        // Primera comanda que leemos
        if (mozoActual == -1)
        {
            mozoActual = comanda.idMozo;
        }

        // Si cambia el mozo, mostramos el resumen del anterior
        if (comanda.idMozo != mozoActual)
        {
            cout << "Mozo: " << mozoActual << endl;
            cout << "Productos vendidos: " << productosVendidos << endl;
            cout << "Comision: $" << comisionMozo << endl;
            cout << endl;

            // Empezamos a acumular para el nuevo mozo
            mozoActual = comanda.idMozo;
            productosVendidos = 0;
            comisionMozo = 0;
        }

        // Acumulamos los datos del mozo actual
        productosVendidos += comanda.cantidad;
        comisionMozo += comanda.comision;

        // Acumulamos el total de productos vendidos por todo el buffet
        totalProductos += comanda.cantidad;

        // Dato extra: calculamos la facturacion total
        float precio = buscarPrecio(comanda.codigoProducto);

        if (precio != -1)
        {
            totalFacturado += precio * comanda.cantidad;
        }
    }

    // Mostramos el ultimo mozo
    if (mozoActual != -1)
    {
        cout << "Mozo: " << mozoActual << endl;
        cout << "Productos vendidos: " << productosVendidos << endl;
        cout << "Comision: $" << comisionMozo << endl;
        cout << endl;
    }

    // Datos finales de toda la semana
    cout << "Total de productos vendidos por el buffet: "
         << totalProductos << endl;

    cout << "(Dato extra) Facturacion total de la semana: $"
         << totalFacturado << endl;

    fclose(archivo);
}


