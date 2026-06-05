#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>

#define FILAS 15
#define COLUMNAS 20
#define MAX_BALAS 5
#define MAX_ENEMIGOS 8

typedef struct {
    int x, y;
    bool activo;
} Bala;

typedef struct {
    int x, y;
    bool activo;
} Enemigo;

// Variables globales del juego
char tablero[FILAS][COLUMNAS];
int naveX;
Bala balas[MAX_BALAS];
Enemigo enemigos[MAX_ENEMIGOS];
int puntaje;
int enemigosRestantes;
bool juegoActivo;
int movimientosTotales = 0; // Para el reporte de memoria

// Inicializa el juego
void inicializarJuego() {
    naveX = COLUMNAS / 2;
    puntaje = 0;
    juegoActivo = true;
    enemigosRestantes = MAX_ENEMIGOS;

    // Inicializar balas
    for (int i = 0; i < MAX_BALAS; i++) {
        balas[i].activo = false;
    }

    // Inicializar enemigos (fila superior)
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        enemigos[i].x = i * 2 + 2; 
        enemigos[i].y = 1;
        enemigos[i].activo = true;
    }
}

// Dibuja el tablero, nave, enemigos y balas
void dibujarTablero() {
    system("cls"); // Limpia la consola (usa "clear" si compilas en Linux/Mac)
    
    // Llenar tablero vacío
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            tablero[i][j] = ' ';
        }
    }

    // Dibujar nave
    tablero[FILAS - 1][naveX] = 'X';

    // Dibujar balas
    for (int i = 0; i < MAX_BALAS; i++) {
        if (balas[i].activo) {
            tablero[balas[i].y][balas[i].x] = '|';
        }
    }

    // Dibujar enemigos
    for (int i = 0; i < MAX_ENEMIGOS; i++) {
        if (enemigos[i].activo) {
            tablero[enemigos[i].y][enemigos[i].x] = 'O';
        }
    }

    // Imprimir tablero
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%c", tablero[i][j]);
        }
        printf("\n");
    }

    printf("Puntaje: %d\n", puntaje);
}

// Mover balas hacia arriba y detectar colisiones
void actualizarJuego() {
    // Mover balas
    for (int i = 0; i < MAX_BALAS; i++) {
        if (balas[i].activo) {
            balas[i].y--;
            if (balas[i].y < 0) {
                balas[i].activo = false;
            }
        }
    }

    // Colisiones (Balas vs Enemigos)
    for (int b = 0; b < MAX_BALAS; b++) {
        if (balas[b].activo) {
            for (int e = 0; e < MAX_ENEMIGOS; e++) {
                if (enemigos[e].activo && balas[b].x == enemigos[e].x && balas[b].y == enemigos[e].y) {
                    enemigos[e].activo = false;
                    balas[b].activo = false;
                    puntaje += 100;
                    enemigosRestantes--;
                }
            }
        }
    }

    // Detectar victoria
    if (enemigosRestantes <= 0) {
        system("cls");
        printf("¡VICTORIA! Puntaje final: %d\n", puntaje);
        printf("Presiona 'r' para Reiniciar o 'q' para Salir: ");
        char opcion;
        do {
            opcion = getch();
        } while (opcion != 'r' && opcion != 'q');
        
        if (opcion == 'r') inicializarJuego();
        else juegoActivo = false;
    }
}

// Disparar una bala
void disparar() {
    for (int i = 0; i < MAX_BALAS; i++) {
        if (!balas[i].activo) {
            balas[i].activo = true;
            balas[i].x = naveX;
            balas[i].y = FILAS - 2;
            break;
        }
    }
}

// Imprimir reporte de memoria y estado
void reporteMemoria() {
    printf("\n--- REPORTE DE ESTADO Y MEMORIA ---\n");
    printf("Tamanio de estructuras:\n");
    printf("- Estructura Bala: %lu bytes\n", sizeof(Bala));
    printf("- Estructura Enemigo: %lu bytes\n", sizeof(Enemigo));
    printf("Uso de memoria del Tablero: %lu bytes\n", sizeof(tablero));
    printf("Estado del juego: %s\n", juegoActivo ? "Activo" : "Finalizado");
    printf("Enemigos restantes: %d / %d\n", enemigosRestantes, MAX_ENEMIGOS);
    printf("Movimientos de nave registrados: %d\n", movimientosTotales);
    printf("------------------------------------\n");
    printf("Presiona cualquier tecla para continuar...");
    getch();
}

int main() {
    inicializarJuego();

    while (juegoActivo) {
        dibujarTablero();
        actualizarJuego();
        
        if (!juegoActivo) break;

        // Captura de teclado
        if (_kbhit()) {
            char tecla = _getch();
            switch (tecla) {
                case 'a':
                case 'A': // Izquierda
                    if (naveX > 0) {
                        naveX--;
                        movimientosTotales++;
                    }
                    break;
                case 'd':
                case 'D': // Derecha
                    if (naveX < COLUMNAS - 1) {
                        naveX++;
                        movimientosTotales++;
                    }
                    break;
                case ' ': // Espacio para disparar
                    disparar();
                    break;
                case 'm':
                case 'M': // Reporte
                    reporteMemoria();
                    break;
                case 'q':
                case 'Q': // Salir
                    juegoActivo = false;
                    break;
            }
        }
        
        // Pequeña pausa para controlar la velocidad del juego (100 ms)
        for(int i=0; i<30000000; i++); 
    }

    printf("Juego terminado. ¡Hasta luego!\n");
    return 0;
