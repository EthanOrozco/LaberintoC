#ifndef libreria_h
#define libreria_h

//define section
typedef enum {
    JUGANDO,
    GANO,
    PERDIO, 
    MENU,
} estadoJuego;

void dibujarMapa();
int generarLaberinto();


#endif /* libreria_h */