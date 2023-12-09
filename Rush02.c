#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

/*
IMPORTANTE!!!
CERRAR EL ARCHIVO Y HACER FREE A MALLOC!!
*/

//Idea: Incorporar contador de malloc y free y de fichero abierto y cerrado.

int main(int argc, char **argv) {
    if (argc > 3) {
        write(1, "Uso incorrecto. Proporcione un archivo como argumento.\n", 56);
        exit(EXIT_FAILURE);
    }

    // Nombre del archivo
	// anadir if para cuando argc sea solo 2 o 3
    const char *nombreArchivo = argv[1];

    // Intenta abrir el archivo en modo de solo lectura (O_RDONLY)
    int descriptorArchivo = open(nombreArchivo, O_RDONLY);

    // Verifica si el archivo se abrió correctamente
    if (descriptorArchivo == -1) {
        write(1, "No se pudo abrir el archivo\n", 28);
        exit(EXIT_FAILURE);
    }

    // Tamaño del búfer
    const size_t tamanoBuffer = 1000;
    char *buffer = (char *)malloc(tamanoBuffer);

    // Verifica si la asignación de memoria fue exitosa
    if (buffer == NULL) {
        write(1, "Error al asignar memoria\n", 25);
        close(descriptorArchivo);
        exit(EXIT_FAILURE);
    }
   
	///////////////////////////////////////////////////////////////////////////////
	// SSIZE_T tipo de dato que representa conteo de bytes es signed size. <0 errores
	//
	// comportamiento:
	// fd: descriptor de archivo (nombre archivo)
	// buf: puntero al bufer donde se almacenaram los datos
	// count: numero de bytes para leer
	// read:
	// ssize_t read(int fd, void *buf, size_tcount);
	// write:
	// ssize_t write(int fd, const void *buf, size_t count);
	//
    // Lee y muestra el contenido del archivo
    ssize_t bytesLeidos;
    while ((bytesLeidos = read(descriptorArchivo, buffer, tamanoBuffer)) > 0) {
		//Mientras no de error la lectura o llegue al final, entrara aqui
        //write(1, buffer, bytesLeidos);
    }

    // Verifica si hubo un error durante la lectura
    if (bytesLeidos == -1) {
        write(1, "Error al leer el archivo\n", 26);
        free(buffer);
        close(descriptorArchivo);
        exit(EXIT_FAILURE);
    }

    // Contar filas y columnas
    int i = 0, col = 0, fil = 0, columnas = 0;

    while (buffer[i] != '\0') {
        if (buffer[i] == '\n') {
            fil++;
            if (col > columnas)
                columnas = col;
            col = -1;
        }
        i++;
        col++;
    }
/*
    char cfil = fil + '0';
    char ccol = columnas + '0';

    write(1, &cfil, 1);
    write(1, "\n", 1);
    write(1, &ccol, 1);
    write(1, "\n", 1);
*/
    // Ahora, asignamos memoria para la matriz, despues de obtener filas y columnas en el paso anterior
	// Implementar un while en lugar de for para aprobar
    char **matrix = (char **)malloc(fil * sizeof(char *));
    for (int i = 0; i < fil; i++) {
        matrix[i] = (char *)malloc(columnas * sizeof(char));
    }

    // Resto del código para llenar la matriz con el contenido del buffer
	// A partir de aqui se puede separar en otro archivo
    int x = 0, y = 0;

    i = 0; // Reinicia el índice
	//Copia el buffer en una matriz
    while (buffer[i] != '\0') {
        if (buffer[i] != ' ') {
            if (buffer[i] != '\n') {
                matrix[x][y] = buffer[i];
                y++;
            }
            if (buffer[i] == '\n') {
                x++;
                y = 0;
            }
        }
        i++;
    }

    // Imprime la matriz, no sirve para nada, solo comprovacion
    for (int i = 0; i < fil; i++) {
        for (int j = 0; j < columnas; j++) {
            write(1, &matrix[i][j], 1);
        }
        write(1, "\n", 1);
    }

    // Libera la memoria asignada para la matriz
    for (int i = 0; i < fil; i++) {
        free(matrix[i]);
    }
    free(matrix);

    // Libera la memoria asignada al búfer
    free(buffer);

    // Cierra el archivo después de usarlo
    close(descriptorArchivo);

    exit(EXIT_SUCCESS);
}
