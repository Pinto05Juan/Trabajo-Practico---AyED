# TP Integrador — Buffet Albert Einstein

Trabajo Práctico grupal de **Algoritmos y Estructuras de Datos** (UTN FRBA) —
unidad de **archivos binarios en C/C++**.

> **Grupo:** _no se asignaron numeros de grupo_
> **Integrantes:** _(nombre — usuario de GitHub, uno por línea)_
_Juan Bautista Alamos - Juanba290806_
_Tomas Aranda - tomasaranda333_
_Tiago Nicosia - tnicosia7_
_Pinto Juan - Pinto05Juan_
_Tomás Loiterstein - tloiterstein_

**Aclaración sobre el nombre de Normalización.cpp**
El programa de normalización se encuentra en el archivo Normalización.cpp, con N mayúscula, en lugar de normalización.cpp, que es el nombre indicado en la consigna.

No nos dimos cuenta cuando lo creamos ya que Windows normalmente no distingue mayúsculas y minúsculas en los nombres de archivos. Para evitar modificar el historial de commits del repositorio, se decidió conservar el nombre actual.
El profesor fue consultado sobre esta diferencia y confirmó que no es necesario renombrar el archivo.

**Aclaración sobre el nombre de Ventas.cpp**
El programa de ventas se encuentra en el archivo Ventas.cpp (con V mayúscula) en lugar de ventas.cpp (con v minúscula), que es el nombre indicado en la consigna.

No nos dimos cuenta cuando lo creamos ya que Windows normalmente no distingue mayúsculas y minúsculas en los nombres de archivos. Para evitar modificar el historial de commits del repositorio, se decidió conservar el nombre actual.
El profesor fue consultado sobre esta diferencia y confirmó que no es necesario renombrar el archivo.



# Normalización:

_Responsables:_ 
_Juan Bautista Alamos - Juanba290806_
_Tomas Aranda - tomasaranda333_

Este es el primer programa a ejecutar ya que se encarga de dejar listos los archivos de datos que van a ser utilizados por los demás programas. 

**Explicación breve del funcionamiento**

El programa se encarga de inicializar el nuevo sistema. Procesa el registro viejo y desordenado de comandas históricas y deja listos los archivos estructurados de mozos, comandas-por-dia e inventario para que los otros 3 programas los utilicen.

**Resolución de puntos clave**

-Generación del archivo de mozos: Abrimos el archivo de `mozos.dat` en modo lectura/escritura para que de esta forma estuviéramos seguros de que el archivo estuviera vacío y al mismo tiempo fuera posible buscar información. De esta manera, podíamos buscar al mozo directamente ahí, y en caso de encontrarlo, sobreescribimos el registro para sumar la comisión actualizada en ese momento.

-Codificación de la clave de cada mozo: Utilizamos un algoritmo de corrimiento con k=7, sumandole 7 al valor ASCII de cada carácter de la contraseña de cada mozo, las cuales tenían como base su ID, que arrancaba en 100 y se les sumaba de a 1 a medida que se iban cargando los mozos.

-Actualizar el stock sin rehacer el inventario: en vez de leer todo `inventario.dat`, modificarlo en memoria y reescribir el archivo entero, se abre con "rb+" y se recorre registro por registro. Cuando se encuentra un producto con ventas acumuladas, se usa fseek para retroceder exactamente al inicio de ese registro y fwrite para sobreescribir solo esos bytes, dejando el resto del archivo intacto

-Elección del método de ordenamiento: Para ordenar cada planilla diaria por idMozo use el método de burbuja. Elegí este método ya que es simple de implementar en archivos binarios. Además, la cantidad de ventas por dia no es tan grande como para poner a analizar la eficiencia frente a otros métodos de ordenamiento.

-Evitar stock negativo: si la cantidad vendida acumulada de un producto supera el stock disponible, decidí dejar el stockActual en 0 en lugar de permitir un valor negativo, ya que esto no presenta una situación real de inventario

# Ventas:

_Responsable: Tiago Nicosia - tnicosia7_

**Explicación breve del funcionamiento:**
El programa permite cargar las ventas de un día (por cada venta, se pide el logueo del mozo, el código de producto y la cantidad vendida). Primero solicita la fecha y genera el nombre correspondiente de la planilla (`comandas_dd-mm-aaaa.dat`). Si la planilla no existe, se crea; si ya existe, se agregan las nuevas ventas.

Ventas.cpp es el segundo programa a ejecutar. Permite agregar ventas a las comandas creadas en Normalizacion.cpp o crear nuevas comandas.

**Resolución de puntos clave**
-Planilla diaria: se abre `comandas_dd-mm-aaaa.dat` en modo ab+ porque necesitamos agregar las nuevas ventas al final del archivo y, en caso de que la planilla todavía no exista, crearla. Al finalizar la carga, el archivo se cierra para luego ordenarlo.

-Login: se abre `mozos.dat` en modo rb porque solo necesitamos leer los datos del mozo, sin modificarlos. Se busca el mozo utilizando su ID para calcular directamente su posición (ID - 100). Esto se puede hacer ya que los mozos están ordenados por ID (consecutivos, es decir, sin huecos) y se conoce el valor inicial. Luego se codifica la clave ingresada y se compara carácter por carácter con la almacenada.

-Codificación de claves: se recorre la clave carácter por carácter y se suma K = 7 al código de cada carácter. Para verificarla se vuelve a aplicar el mismo corrimiento a la clave ingresada y se compara con la guardada.

-Búsqueda de productos: se abre `inventario.dat` en modo rb porque solo necesitamos consultar los productos sin modificarlos durante la búsqueda. Como está ordenado por código, se utiliza búsqueda binaria. En cada paso se accede mediante fseek al registro del medio y se descarta la mitad del archivo que no puede contener el código buscado. En este caso, no se puede realizar búsqueda PUP ya que los códigos no son consecutivos.

-Actualización del inventario: se abre `inventario.dat` en modo rb+ porque necesitamos tanto leer el registro del producto como modificarlo. Una vez encontrada la posición, se lee el registro, se resta la cantidad vendida a stockActual y se vuelve a escribir en esa misma posición con el nuevo valor.

-Comisión: se abre `mozos.dat` en modo rb+ porque necesitamos leer el registro del mozo y modificar su totalComision. La comisión se obtiene a partir del precio del producto, la cantidad vendida y la tasa de comisión del 10%. Luego se suma al totalComision del mozo y se actualiza su registro en `mozos.dat`.

-Ordenamiento de la planilla: se abre la planilla en modo rb+ porque necesitamos leer los registros para compararlos y también sobrescribirlos al realizar los intercambios. Se recorre la planilla buscando en cada posición la venta con menor idMozo. Si se encuentra una, se intercambia con la venta de la posición actual, realizando el ordenamiento por selección directamente sobre el archivo.

# Cierre
_Responsable: Pinto Juan - Pinto05Juan_

**Explicación breve del funcionamiento:**
El programa permite juntar las comandas diarias generadas anteriormente para crear varias comandas semanales de un mes correspondiente. El programa pide al usuario que ingrese un mes y año, junta las planillas de cada semana en un mes y genera `comandas_semana_sX-mm.dat`, una por cada semana.

**Resolución de puntos clave:**
-Técnica usada para juntar las comandas: Como las comandas diarias ya vienen ordenadas por un campo clave (idMozo), usamos apareo para juntarlas correctamente sin perder ese orden.

-Un día sin comandas (sin ventas): Para los casos de días sin ventas, esto se maneja mediante un condicional verificando si existe el nombre de ese archivo. Al no encontrar ese archivo, seguimos iterando al proximo dia.

-Corte de 7 días para una semana: Para este caso, se utilizó un diseño de semana basado en 7 días consecutivos. Esto se decidió para que ninguna semana se cruce de un mes a otro, ya que el nombre del archivo incluye el mes (sX-mm), Por eso al llegar al último día del mes, se corta la semana aunque no se hayan completado los 7 días.

-Uso de archivos temporales para el apareo: Se usaron dos archivos temporales (temporal1.dat, temporal2.dat) que van alternando su función de guardar y escribir la información en cada apareo sucesivo y se eliminan al terminar el programa

# RESUMEN
_Responsable: Tomás Loiterstein - tloiterstein_

**Explicación breve del funcionamiento:**
El programa de resumen se ejecuta al finalizar el cierre semanal, ya que necesita que previamente se haya generado la planilla semanal `comandas_semana_sX-mm.dat`. Primero solicita el número de semana y el mes para generar el nombre del archivo que debe leer. Luego abre esa planilla en modo lectura y recorre todas las comandas.

Como las comandas de la planilla semanal ya se encuentran ordenadas por `idMozo`, podemos recorrerlas secuencialmente y acumular para cada mozo la cantidad de productos vendidos y la comisión correspondiente. Cuando cambia el `idMozo`, mostramos el resumen del mozo anterior y comenzamos a acumular los datos del siguiente.

Al finalizar, mostramos el total de productos vendidos por el buffet durante toda la semana. Además, como funcionalidad extra, calculamos y mostramos la facturación total de la semana.

**Resolución de puntos clave**
-Generación del nombre del archivo: solicitamos el número de semana y el mes y armamos el nombre con el formato `comandas_semana_sX-mm.dat`, que es el archivo generado por el programa de cierre y que necesitamos leer.

-Lectura de la planilla semanal: abrimos `comandas_semana_sX-mm.dat` en modo `rb` porque solamente necesitamos leer las comandas y no modificar el archivo.

-Aprovechamiento del orden por mozo: la planilla semanal se encuentra ordenada por `idMozo` gracias al proceso de cierre. Por eso no necesitamos ordenar nuevamente ni utilizar estructuras complejas. Guardamos el `idMozo` actual y recorremos las comandas mientras pertenecen al mismo mozo.

-Resumen por mozo: para cada mozo acumulamos `comanda.cantidad` en `productosVendidos` y `comanda.comision` en `comisionMozo`. Cuando encontramos una comanda cuyo `idMozo` es diferente al actual, mostramos los datos acumulados del mozo anterior y reiniciamos los acumuladores para el nuevo mozo.

-Total de productos vendidos: además del acumulado individual de cada mozo, tenemos `totalProductos`, que suma la cantidad de unidades de todas las comandas de la semana. Este es el total solicitado por la consigna: cuántos productos vendió el buffet en toda la semana.

-Comisión: no volvemos a calcular la comisión en este programa. La comisión ya fue calculada anteriormente por `Ventas.cpp` y está guardada dentro de cada `Comanda`, por lo que en `Resumen.cpp` simplemente acumulamos el valor de `comanda.comision`.

-Facturación total como funcionalidad extra: también calculamos la facturación total de la semana. Para hacerlo, buscamos en `inventario.dat` el precio correspondiente al código de cada producto y multiplicamos `precio * cantidad`. Este dato se muestra como información adicional y no reemplaza al total de productos solicitado por la consigna.

-Búsqueda del precio: para obtener el precio de un producto se abre `inventario.dat` en modo `rb` y se recorre hasta encontrar el código correspondiente. Solo necesitamos consultar el archivo, por eso no es necesario modificarlo.

-Cierre del archivo: una vez terminada la lectura de la planilla semanal, cerramos el archivo con `fclose`.








La consigna completa está en **`enunciado.pdf`**. Leela antes de arrancar: el
cliente (Alberto) cuenta su problema a su manera y ustedes tienen que descubrir
cómo resolverlo con lo que vimos de archivos.

## Estructura del repo

```
.
├── enunciado.pdf              # la consigna
├── datos/                     # archivos PROVISTOS por la cátedra (no se tocan)
│   ├── comandas_historicas.dat
│   ├── inventario.dat
│   └── dump_datos_de_prueba.txt   # los dos .dat en texto, para verificar la lectura
├── .gitignore
│
│   # --- estos los crean USTEDES (no vienen en el repo base) ---
├── normalizacion.cpp          # cada uno con su propio main; NO hay main.cpp
├── ventas.cpp
├── cierre.cpp
└── resumen.cpp
```

Los 4 programas los escriben ustedes desde cero: el repo base no trae código.
Créenlos en la raíz del repo (como en el diagrama del enunciado).

## Cómo trabajar

Los programas leen y escriben archivos `.dat` en la carpeta donde se ejecutan.
Los archivos de `datos/` son los **originales** y no se modifican: copiálos a la
carpeta donde vas a compilar y correr, y trabajá sobre esas copias.

```bash
cp datos/comandas_historicas.dat datos/inventario.dat .
```

Compilar y correr cada programa (cada uno tiene su propio `main`, no hay `main.cpp`):

```bash
g++ -O2 -o normalizacion normalizacion.cpp
g++ -O2 -o ventas        ventas.cpp
g++ -O2 -o cierre        cierre.cpp
g++ -O2 -o resumen       resumen.cpp
```

Orden de ejecución (cada programa deja los archivos que usa el siguiente):

```
./normalizacion     # historicas + inventario  ->  mozos.dat, comandas_dd-mm-aaaa.dat (varios), inventario actualizado
./ventas            # carga interactiva de la semana  ->  agrega a las planillas del día
./cierre            # junta los días de la semana      ->  comandas_semana_sX-mm.dat
./resumen           # imprime el resumen por mozo + total del buffet
```

## Verificar que leen bien el binario

`datos/dump_datos_de_prueba.txt` tiene el contenido de los dos archivos provistos
en texto legible. Si tu programa imprime otra cosa al leerlos, revisá los `struct`
(orden de campos / tamaños): tienen que dar `sizeof(ComandaHistorica)=76` y
`sizeof(Producto)=64`.

## Qué se entrega

- Este mismo repo (uno por grupo), con los **4 programas** `.cpp` en la raíz
  (cada uno con su propio `main`, no hay `main.cpp`).
- El **README** con el grupo, los integrantes (nombre + usuario de GitHub) y, si
  hace falta, cualquier aclaración de cómo correrlo.
- Los `.dat` de `datos/` son los **provistos por la cátedra** y ya vienen en el
  repo: no los borren ni agreguen otros. Cualquier `.dat` que ustedes copien a la
  raíz o generen al correr **no** se versiona (el `.gitignore` ya los deja afuera).
  La cátedra corre sus programas sobre su propio dataset.
- **Commits repartidos:** cada integrante tiene que tener commits propios a lo
  largo del trabajo. El historial es parte de lo que se mira para la defensa
  individual (no vale un único commit final ni que suba todo una sola persona).
