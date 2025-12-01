# tarea-os-memoria-virtual
Repositorio que contiene la tarea 2 para el ramo "Sistemas Operativos".

**Integrantes**:

    Benjamín Poblete
    Carlos Salinas
    Khristian Villalobos

**Como usar el programa respecto a la Parte 1:**

Actividad 1:

    Hay 2 formas de compilar el código:

    1) "gcc testBarrera.c -o testBarrera"
    2) "gcc -pthread testBarrera.c -o testBarrera"

    Donde "-pthread" indica al compilador que se usen las bibliotecas
    POSIX.
    
    Luego se puede ejecutar el programa con:

    "./testBarrera"

Actividad 2:

    Luego de compilar el código con "gcc barrera.c -o barrera",
    hay 2 formas de ejecutar el programa.

    1) "./barrera"   (sin argumentos)
    2) "./barrera <N> <E>".

    Donde <N> es la cantidad de hebras y <E> es la cantidad de etapas.

    Por defecto, la cantidad de hebras será 5 y la cantidad de etapas será 4
    si no se ingresan argumentos al ejecutar.

**Como usar el programa respecto a la Parte 2:**

    Luego de compilar el código con "gcc sim.c -o sim", 
    hay 2 formas de ejecutar el programa.

    !) "./sim <n_marcos> <t_marco> <traza.txt>"
    2) "./sim <n_marcos> <t_marco> --verbose <traza.txt>"

    Donde <n_marcos> es la cantidad de marcos,
    <t_marco> es el tamaño de cada marco,
    <traza.txt> el archivo donde se leen las referencias
    y "verbose" para mostrar información adicional al ejecutar.

**Notas:**

    Se pueden usar los archivos "trace1.txt" y "trace2.txt"
    para usarlas como referencias para el programa de la parte 2.
