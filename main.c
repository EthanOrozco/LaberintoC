#include <stdio.h>
#include "libreria.h"
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define SIZE 31
// strcpy
int laberinto[SIZE][SIZE] = {};

void movement(int *x, int *y)
{
    int new_x = *x;
    int new_y = *y;

    // Espera a que se presione una tecla de dirección o WASD
    while (1)
    {
        if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W'))
        {
            new_y -= 1;
            break;
        }
        else if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S'))
        {
            new_y += 1;
            break;
        }
        else if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A'))
        {
            new_x -= 1;
            break;
        }
        else if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D'))
        {
            new_x += 1;
            break;
        }
        Sleep(10); // Pequeña pausa para evitar uso excesivo de CPU
    }

    // Verifica límites
    if (new_x < 0 || new_x >= SIZE || new_y < 0 || new_y >= SIZE)
    {
        printf("Fuera de límites\n");
        return;
    }
    // Verifica si es muro
    if (laberinto[new_y][new_x] == 1)
    {
        printf("No puedes pasar por aquí\n");
        return;
    }
    // Recoge moneda
    if (laberinto[new_y][new_x] == 3)
    {
        printf("Has recogido una moneda\n");
        laberinto[new_y][new_x] = 0;
    }
    // Llega a la salida
    if (laberinto[new_y][new_x] == 9)
    {
        printf("Has llegado a la salida\n");
        exit(0);
    }
    // Actualiza la posición del jugador en el mapa
    laberinto[*y][*x] = 0; // Borra la posición anterior
    *x = new_x;
    *y = new_y;
    laberinto[*y][*x] = 2; // Nueva posición
    system("cls");         // Limpia la pantalla
    dibujarMapa();         // Redibuja el mapa
}

void generarmonedas()
{
    for (int y = 1; y < SIZE - 1; y++)
    {
        for (int x = 1; x < SIZE - 1; x++)
        {
            // Solo poner monedas en espacios vacíos
            if (laberinto[y][x] == 0)
            {
                int randnum = rand() % 10;
                if (randnum == 0)
                {
                    laberinto[y][x] = 3; // 10% de probabilidad de moneda
                }
            }
        }
    }
}

// Change the signature to return the player position via pointers
int generarLaberinto(int *playerpositionx, int *playerpositiony)
{
    *playerpositionx = 0;
    *playerpositiony = 0;
    FILE *f;
    f = fopen("lab1.txt", "r");
    if (f == NULL)
    {
        printf("Error al abrir el archivo laberinto.txt\n");
        return 0;
    }
    for (int y = 0; y < SIZE; y++)
    {
        int x = 0;
        while (x < SIZE)
        {
            int num = fgetc(f);
            if (num == EOF)
            {
                fclose(f);
                return 0;
            }
            if (num == '\n' || num == '\r')
            {
                continue;
            }
            if (num == '1')
            {
                laberinto[y][x] = 1; // muro
                x++;
            }
            else if (num == '0')
            {
                laberinto[y][x] = 0; // espacio vacío
                x++;
            }
            else if (num == '2')
            {
                laberinto[y][x] = 2; // jugador
                *playerpositionx = x;
                *playerpositiony = y;
                x++;
            }
            else if (num == '3')
            {
                laberinto[y][x] = 3; // moneda
                x++;
            }
            else if (num == '9')
            {
                laberinto[y][x] = 9; // salida
                x++;
            }
            // ignora cualquier otro caracter
        }
    }
    fclose(f);
    return 1;
}

void dibujarMapa()
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            if (laberinto[y][x] == 2)
            {
                printf("\033[36m@ \033[0m"); // jugador en azul claro
            }
            else if (laberinto[y][x] == 3)
            {
                printf("\033[33m$ \033[0m"); // moneda en amarillo
            }
            else if (laberinto[y][x] == 9)
            {
                printf("\033[32mS \033[0m"); // salida en verde
            }
            else if (laberinto[y][x] == 1)
            {
                printf("# "); // muro
            }
            else
            {
                printf("  "); // espacio vacío
            }
        }
        printf("\n");
    }
}

void printmapa()
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            printf("%d ", laberinto[y][x]);
        }
        printf("\n");
    }
}

int main(void)
{
    srand(time(NULL)); // Inicializa la semilla para la generación aleatoria
    system("cls");
    int playerpositionx, playerpositiony;
    generarLaberinto(&playerpositionx, &playerpositiony);
    generarmonedas();
    dibujarMapa();
    while (1)
    {
        movement(&playerpositionx, &playerpositiony);
        Sleep(100); // Pausa para evitar uso excesivo de CPU
    }
    printf("x: %d y: %d\n", playerpositionx, playerpositiony);
    return 0;
}