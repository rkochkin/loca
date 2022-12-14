#include <stdio.h>
#include <string.h>

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*Для Linux и Windows*/
#else
#include <GLUT/GLUT.h> /*Для Mac OS*/
#endif


#define X_RES 800
#define Y_RES 800
#define X_N 100
#define Y_N 100
#define TIME_DELAY 100

typedef int (*act_t)(void *, void *, int self_x, int self_y);

struct __cell_t {
    act_t act;
    int pow;
};

typedef struct __cell_t cell_t;

typedef struct {
    cell_t cell[X_N][Y_N];
    int max_x;
    int max_y;
    int impression;
} life_map_t;

void life_init(life_map_t *map, act_t act, int pow, int impression) {
    int i = 0, j = 0;

    map->max_x = sizeof(map->cell) / sizeof(map->cell[0]);
    map->max_y = sizeof(map->cell[0]) / sizeof(map->cell[0][0]);

    map->impression = impression;

    for (j = 0; j < Y_N; j++)
        for (i = 0; i < X_N; i++) {
            map->cell[i][j].act = act;
            map->cell[i][j].pow = pow;
        }
}

void life_origin() {
}

void life_quant(life_map_t *map_in, life_map_t *map_out) {
    int i = 0, j = 0;
    for (j = 0; j < Y_N; j++)
        for (i = 0; i < X_N; i++) {
            //printf("Quant pecessing: %d %d\n", i,j);
            map_in->cell[i][j].act((void *) map_in, (void *) map_out, i, j);
        }
}

int life_act(void *mapin_v, void *mapout_v, int self_x, int self_y) {
    life_map_t *map_in = (life_map_t *) mapin_v;
    life_map_t *map_out = (life_map_t *) mapout_v;

    int sum = 0;
    int i, j;
    int rx, ry;
    for (j = -1; j <= 1; j++)
        for (i = -1; i <= 1; i++) {
            if (i == 0 && j == 0) {

            } else {
                rx = self_x + i;
                ry = self_y + j;
                //printf("Life act rx ry %d %d\n", rx,ry);
                if (rx >= 0 && rx < map_in->max_x && ry >= 0 && ry < map_in->max_y)
                    sum += map_in->cell[rx][ry].pow;
                else
                    sum += map_in->impression;
            }
        }
    //printf("sum  %d \n", sum);
    if (map_in->cell[self_x][self_y].pow > 0) {   // Если клетка живая
        if (sum == 2 || sum == 3)                 // Если есть 2 или 3 живые соседки
            map_out->cell[self_x][self_y].pow = 1;// то клетка продолжает жить
        else
            map_out->cell[self_x][self_y].pow = 0;// иначе умирает
    } else {                                      // Если пусто
        if (sum == 3)                             // Если есть ровно 3 живые соседки
            map_out->cell[self_x][self_y].pow = 1;//зарождается жизнь в клетке
        else
            map_out->cell[self_x][self_y].pow = 0;
    }
    return 0;
}


void reshape(int w, int h);
void display();
void keyPressed(unsigned char key, int x, int y);
void mouseEvent(int button, int state, int x, int y);
void display_life_map(life_map_t *map);
void timerEvent(int value);

life_map_t __one_map;
life_map_t __second_map;

int main(int argc, char *argv[]) {
    life_init(&__one_map, life_act, 0, 0);
    memcpy(&__second_map, &__one_map, sizeof(__one_map));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*Включаем двойную буферизацию и четырехкомпонентный цвет*/

    glutInitWindowSize(X_RES, Y_RES);
    glutCreateWindow("OpenGL Life Game");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutMouseFunc(mouseEvent);
    glutMainLoop();

    return 0;
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    int i;
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(0, 0);
    glVertex2i(X_RES, 0);

    glVertex2i(X_RES, 0);
    glVertex2i(X_RES, Y_RES);

    glVertex2i(X_RES, Y_RES);
    glVertex2i(0, Y_RES);

    glVertex2i(0, Y_RES);
    glVertex2i(0, 0);

    glColor3f(0.2, 0.2, 0.2);

    for (i = 0; i < X_RES; i += X_RES / X_N) {

        glVertex2i(i, 0);
        //glColor3f(0.2, 0.2, 0.2);
        glVertex2i(i, Y_RES);
    }

    for (i = 0; i < Y_RES; i += Y_RES / Y_N) {
        //glColor3f(1.0, 1.0, 1.0);
        glVertex2i(0, i);
        //glColor3f(1.0, 1.0, 1.0);
        glVertex2i(X_RES, i);
    }
    /*glColor3f(0.0, 1.0, 0.0);
                glVertex2i(550, 150);
                glColor3f(1.0, 0.0, 0.0);
                glVertex2i(550, 450);*/
    glEnd();
    display_life_map(&__one_map);
    glutSwapBuffers();
}

unsigned char __start = 0;

void keyPressed(unsigned char key, int x, int y) {

    printf("Hello KBD!!! Mouse (%d, %d)\n", x, y);

    __start ^= 1;
    glutTimerFunc(500, timerEvent, 0);
}

void mouseEvent(int button, int state,
                int x, int y) {
    int nx, ny;
    life_map_t *map = &__one_map;
    //printf("Hello Mouse!!! Mouse (%d, %d, %d, %d)\n",button, state, x,y);
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        nx = x / (X_RES / X_N);
        ny = y / (Y_RES / Y_N);
        printf("Hello Mouse!!! Mouse (%d, %d)\n", nx, ny);
        if (nx < map->max_x && ny < map->max_y) map->cell[nx][map->max_y - 1 - ny].pow ^= 1;
        display();
    }
}

void display_life_map(life_map_t *map) {
    int i = 0, j = 0;
    for (j = 0; j < map->max_y; j++)
        for (i = 0; i < map->max_x; i++) {
            if (map->cell[i][j].pow > 0) {
                glBegin(GL_QUADS);
                glColor3f(1.0, 1.0, 1.0);
                glVertex2i(i * X_RES / X_N, j * Y_RES / Y_N);
                glVertex2i((i + 1) * X_RES / X_N, j * Y_RES / Y_N);
                glVertex2i((i + 1) * X_RES / X_N, (j + 1) * Y_RES / Y_N);
                glVertex2i(i * X_RES / X_N, (j + 1) * Y_RES / Y_N);
                glEnd();
            }
        }
}

void timerEvent(int value) {
    if (__start) {
        life_quant(&__one_map, &__second_map);
        memcpy(&__one_map, &__second_map, sizeof(__one_map));
        display();
        glutTimerFunc(TIME_DELAY, timerEvent, 1);
    }
}