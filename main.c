#include <stdio.h>
#include "libreria.h"
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>

#define SIZE 31
// strcpy
int laberinto[SIZE][SIZE] = {};

int movement(int *x, int *y, int *acumulador_pts, char *mensaje, const char *nombre)
{
    int new_x = *x;
    int new_y = *y;
    mensaje[0] = '\0'; // Limpia el mensaje

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

    if (new_x < 0 || new_x >= SIZE || new_y < 0 || new_y >= SIZE)
    {
        strcpy(mensaje, "Fuera de límites");
        return 0;
    }
    if (laberinto[new_y][new_x] == 1)
    {
        strcpy(mensaje, "No puedes pasar por aquí");
        return 0;
    }
    if (laberinto[new_y][new_x] == 3)
    {
        strcpy(mensaje, "Has recogido una moneda");
        laberinto[new_y][new_x] = 0;
        *acumulador_pts += 200;
    }
    if (laberinto[new_y][new_x] == 9)
    {
        strcpy(mensaje, "Has llegado a la salida");
        *acumulador_pts += 100;
        system("cls");
        dibujarMapa();
        printf("%s\n", mensaje);
        printf("Tu puntaje final es: %d\n", *acumulador_pts);

        FILE *f = fopen("puntajes.txt", "a");
        if (f == NULL)
        {
            printf("Error al abrir el archivo de puntajes.\n");
        }
        else
        {
            fprintf(f, "Puntaje de %s: %d\n", nombre, *acumulador_pts);
            fclose(f);
        }
        // En vez de exit(0), retorna un valor especial para indicar que se llegó a la salida
        *x = new_x;
        *y = new_y;
        laberinto[*y][*x] = 2;
        return 1;
    }
    laberinto[*y][*x] = 0;
    *x = new_x;
    *y = new_y;
    laberinto[*y][*x] = 2;
    system("cls");
    dibujarMapa();
    printf("Puntaje: %d\n", *acumulador_pts);
    if (mensaje[0] != '\0')
        printf("%s\n", mensaje);
    return 0;
}

void menu_usuario(char *nombre, size_t nombre_size)
{
    printf("╔════════════════════════════════╗\n");
    printf("║    Bienvenido al Laberinto     ║\n");
    printf("╠════════════════════════════════╣\n");
    printf("║      Introduce tu nombre:      ║\n");
    printf("║      _____________________     ║\n");
    printf("║      |                   |     ║\n");
    printf("║      |                   |     ║\n");
    printf("║      |___________________|     ║\n");
    printf("║                                ║\n");
    printf("║  Presiona ENTER para continuar ║\n");
    printf("╚════════════════════════════════╝\n");

    printf("\033[7;15H");

    fgets(nombre, nombre_size, stdin); // Lee el nombre del usuario
    nombre[strcspn(nombre, "\n")] = 0; // Elimina el salto de línea

    getchar(); // Espera a que el usuario presione ENTER

    printf("\n\n\n\n\n\nBienvenido %s! \n", nombre);
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
                int randnum = rand() % 15;
                if (randnum == 0)
                {
                    laberinto[y][x] = 3; // probabilidad de 1/50 de poner una moneda
                }
            }
        }
    }
}

int generarLaberinto(int *playerpositionx, int *playerpositiony, const char *filename)
{
    *playerpositionx = 0;
    *playerpositiony = 0;
    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Error al abrir el archivo %s\n", filename);
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

int elegirLaberinto(int *playerpositionx, int *playerpositiony)
{
    int laberintoElegido;
    const char *filename = NULL;
    printf("Elige el laberinto (1-3): ");
    scanf("%d", &laberintoElegido);
    switch (laberintoElegido)
    {
    case 1:
        filename = "lab1.txt";
        break;
    case 2:
        filename = "lab2.txt";
        break;
    case 3:
        filename = "lab3.txt";
        break;
    default:
        printf("Labirinto no válido.\n");
        return 0;
    }
    return generarLaberinto(playerpositionx, playerpositiony, filename);
}

void highscores()
{
    FILE *f = fopen("puntajes.txt", "r");
    if (f == NULL)
    {
        printf("Error al abrir el archivo de puntajes.\n");
        return;
    }
    char line[100];
    printf("╔════════════════════════════════╗\n");
    printf("║           PUNTAJES             ║\n");
    printf("╠════════════════════════════════╣\n");
    while (fgets(line, sizeof(line), f))
    {
        printf("%s", line);
    }
    fclose(f);
    printf("╚════════════════════════════════╝\n");
}
int main(void)
{
    srand(time(NULL)); // Inicializa la semilla para la generación aleatoria
    system("chcp 65001");
    system("cls");
    int playerpositionx, playerpositiony;
    int acumulador_pts = 0;
    char mensaje[100] = "";
    char nombre[32] = "";
    menu_usuario(nombre, sizeof(nombre));
    elegirLaberinto(&playerpositionx, &playerpositiony);
    generarmonedas();
    Sleep(2000); // pausa de 2 segundos para ver el menu inicial
    dibujarMapa();
    while (1)
    {
        int terminado = movement(&playerpositionx, &playerpositiony, &acumulador_pts, mensaje, nombre);
        if (terminado == 1)
            break;
        Sleep(100); // Pausa para evitar uso excesivo de CPU
    }
    getchar();   // Espera a que el usuario presione ENTER
    Sleep(2000); // pausa de 2 segundos para ver el mensaje final

    int opcion;
    do
    {
        printf("1. Jugar de nuevo \n");
        printf("2. Ver puntajes \n");
        printf("3. Cambiar de jugador \n");
        printf("4. Salir \n");
        printf("Elige una opción: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar el buffer de entrada
        switch (opcion)
        {
        case 1:
            system("cls");
            main();
            return 0;
        case 2:
            system("cls");
            highscores();
            break;
        case 3:
            system("cls");
            menu_usuario(nombre, sizeof(nombre));
            break;
        case 4:
            system("cls");
            printf("Gracias por jugar!\n");
            break;
        default:
            system("cls");
            printf("Opción no válida. Saliendo...\n");
            break;
        }
    } while (opcion != 1 && opcion != 4 && opcion != 3);
    return 0;
}