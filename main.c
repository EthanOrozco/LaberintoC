#include <stdio.h>
#include "libreria.h"
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SIZE 30

int laberinto[SIZE][SIZE] = {};

void cargarLaberintoDesdeArchivo(const char* nombreArchivo) {
    FILE *f = fopen(nombreArchivo, "r");
    if (f == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }
    for (int y = 0; y < SIZE; y++) {
        int x = 0;
        while (x < SIZE) {
            int c = fgetc(f);
            if (c == EOF) {
                fclose(f);
                return;
            }
            if (c == '\n' || c == '\r') {
                continue;
            }
            if (c == 1) {
                laberinto[y][x] = 1;
                x++;
            } else if (c == 0) {
                laberinto[y][x] = 0;
                x++;
            } else if (c == 2) {
                laberinto[y][x] = 2;
                x++;
            } else if (c == 9) {
                laberinto[y][x] = 9;
                x++;
            }
            // ignora cualquier otro caracter
        }
    }
    fclose(f);
}

void generarmonedas(){
    for (int y = 1; y < SIZE - 1; y++){
        for (int x = 1; x < SIZE - 1; x++){
            // Solo poner monedas en espacios vacíos
            if (laberinto[y][x] == 0){
                int randnum = rand() % 10;
                if (randnum == 0){
                    laberinto[y][x] = 3; // 10% de probabilidad de moneda
                }
            }
        }
    }
}

void generarLaberinto(){
    
}

void dibujarMapa(){
    for (int y = 0; y < SIZE; y++){
        for (int x = 0; x < SIZE; x++){
            if (y == 0 || y == SIZE - 1){
                printf("# "); // muro superior e inferio
            }else if (laberinto[y][x] == 1){
                printf("#"); // muro normal
            }else if (laberinto[y][x] == 2){
                printf("@ "); // jugador
            }else if (x == SIZE - 1){
                printf("   "); // espacio vacio final
            }else{
                printf("  "); // espacio vacío
            }
        }
        printf("\n");
    }
}

void printmapa(){
    for(int y = 0; y < SIZE; y++){
        for(int x=0; y < SIZE; x++){
            printf("%d ", laberinto[y][x]);
        }
        printf("\n");
    }
}

int main(void)
{
    generarLaberinto();
    printmapa();
    return 0;
}