#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_BALAS 5
#define MAX_ENEMIGOS 5

typedef struct {
    int x, y;
    int activo;
} Objeto;

int main() {
    // Inicializar ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(FALSE);
    nodelay(stdscr, TRUE); // No bloquear la entrada del teclado

    srand(time(NULL));

    // Posición inicial de la nave
    int nave_x = COLS / 2;
    int nave_y = LINES - 3;

    Objeto balas[MAX_BALAS];
    Objeto enemigos[MAX_ENEMIGOS];

    // Inicializar balas y enemigos
    for(int i = 0; i < MAX_BALAS; i++) balas[i].activo = 0;
    for(int i = 0; i < MAX_ENEMIGOS; i++) {
        enemigos[i].activo = 1;
        enemigos[i].x = rand() % (COLS - 4) + 2;
        enemigos[i].y = rand() % 5 + 1;
    }

    int score = 0;
    int juego_activo = 1;

    while(juego_activo) {
        // Limpiar pantalla
        clear();

        // Mostrar Score
        mvprintw(0, 2, "Score: %d", score);

        // Dibujar Nave
        mvprintw(nave_y, nave_x, "^A^");

        // === LÓGICA DE ENEMIGOS ===
        for(int i = 0; i < MAX_ENEMIGOS; i++) {
            if(enemigos[i].activo) {
                mvprintw(enemigos[i].y, enemigos[i].x, "Y");
                enemigos[i].y++; // Bajan

                // Si el enemigo llega al fondo, game over
                if(enemigos[i].y >= LINES - 1) {
                    juego_activo = 0;
                }
            } else {
                // Regenerar enemigo destruido arriba
                enemigos[i].activo = 1;
                enemigos[i].x = rand() % (COLS - 4) + 2;
                enemigos[i].y = 1;
            }
        }

        // === LÓGICA DE BALAS ===
        for(int i = 0; i < MAX_BALAS; i++) {
            if(balas[i].activo) {
                mvprintw(balas[i].y, balas[i].x, "|");
                balas[i].y--; // Suben

                // Desactivar bala si sale de la pantalla
                if(balas[i].y < 1) {
                    balas[i].activo = 0;
                }

                // Colisión con enemigos
                for(int j = 0; j < MAX_ENEMIGOS; j++) {
                    if(enemigos[j].activo && balas[i].x == enemigos[j].x && balas[i].y == enemigos[j].y) {
                        enemigos[j].activo = 0; // Destruir enemigo
                        balas[i].activo = 0;   // Destruir bala
                        score += 100;
                    }
                }
            }
        }

        // === ENTRADA DEL TECLADO ===
        int ch = getch();
        switch(ch) {
            case KEY_LEFT:
                if(nave_x > 2) nave_x--;
                break;
            case KEY_RIGHT:
                if(nave_x < COLS - 3) nave_x++;
                break;
            case ' ': // Barra espaciadora para disparar
                for(int i = 0; i < MAX_BALAS; i++) {
                    if(!balas[i].activo) {
                        balas[i].activo = 1;
                        balas[i].x = nave_x + 1; // Centro de la nave "^A^"
                        balas[i].y = nave_y - 1;
                        break;
                    }
                }
                break;
            case 'q':
                juego_activo = 0;
                break;
        }

        // Refrescar pantalla y pausar (control de velocidad)
        refresh();
        usleep(50000); // 50 milisegundos de pausa (~20 FPS)
    }

    // Pantalla de Game Over
    clear();
    mvprintw(LINES / 2, (COLS - 11) / 2, "GAME OVER");
    mvprintw(LINES / 2 + 1, (COLS - 17) / 2, "Score final: %d", score);
    refresh();
    sleep(3);

    // Restaurar terminal
    endwin();
    return 0;
}
