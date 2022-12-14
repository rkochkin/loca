#include <cstring>
#include <functional>
#include <iostream>

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*Для Linux и Windows*/
#else
#include <GLUT/GLUT.h> /*Для Mac OS*/
#endif

namespace {
    constexpr auto ResolutionX = 800;
    constexpr auto ResolutionY = 800;
    constexpr auto CellNumberX = 100;
    constexpr auto CellNumberY = 100;
    constexpr auto TimeDelay = 100;

    constexpr auto PxLineX = ResolutionX / CellNumberX;
    constexpr auto PxLineY = ResolutionY / CellNumberY;

    unsigned char Start = 0;
}// namespace

struct LifeMap;

using ActionFunc = std::function<int(const LifeMap&, LifeMap&, int self_x, int self_y)>;

struct Cell {
    ActionFunc act;
    int pow = 0;
};

struct LifeMap {
    Cell cell[CellNumberX][CellNumberY];
    int max_x = 0;
    int max_y = 0;
    int impression = 0;
};

void lifeInit(LifeMap *map, const ActionFunc& actFn, int pow, int impression) {
    map->max_x = sizeof(map->cell) / sizeof(map->cell[0]);
    map->max_y = sizeof(map->cell[0]) / sizeof(map->cell[0][0]);

    map->impression = impression;

    for (int j = 0; j < CellNumberY; j++)
        for (int i = 0; i < CellNumberX; i++) {
            map->cell[i][j].act = actFn;
            map->cell[i][j].pow = pow;
        }
}

void lifeOrigin() {
}

void lifeQuant(const LifeMap& map_in, LifeMap& map_out) {
    for (int j = 0; j < CellNumberY; j++)
        for (int i = 0; i < CellNumberX; i++) {
            map_in.cell[i][j].act(map_in, map_out, i, j);
        }
}

int lifeAct(const LifeMap& map_in, LifeMap& map_out, int self_x, int self_y) {

    int sum = 0;
    for (int j = -1; j <= 1; j++)
        for (int i = -1; i <= 1; i++) {
            if (i == 0 && j == 0) {

            } else {
                int rx = self_x + i;
                int ry = self_y + j;
                if (rx >= 0 && rx < map_in.max_x && ry >= 0 && ry < map_in.max_y)
                    sum += map_in.cell[rx][ry].pow;
                else
                    sum += map_in.impression;
            }
        }

    if (map_in.cell[self_x][self_y].pow > 0) {   // Если клетка живая
        if (sum == 2 || sum == 3)                 // Если есть 2 или 3 живые соседки
            map_out.cell[self_x][self_y].pow = 1;// то клетка продолжает жить
        else
            map_out.cell[self_x][self_y].pow = 0;// иначе умирает
    } else {                                      // Если пусто
        if (sum == 3)                             // Если есть ровно 3 живые соседки
            map_out.cell[self_x][self_y].pow = 1;// зарождается жизнь в клетке
        else
            map_out.cell[self_x][self_y].pow = 0;
    }
    return 0;
}


void reshape(int w, int h);
void display();
void keyPressed(unsigned char key, int x, int y);
void mouseEvent(int button, int state, int x, int y);
void displayLifeMap(const LifeMap& map);
void displayNet();
void timerEvent(int value);

LifeMap inMap;
LifeMap outMap;

int main(int argc, char *argv[]) {
    lifeInit(&inMap, lifeAct, 0, 0);
    outMap = inMap;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*Включаем двойную буферизацию и четырехкомпонентный цвет*/

    glutInitWindowSize(ResolutionX, ResolutionY);
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

    displayNet();
    displayLifeMap(inMap);
    glutSwapBuffers();
}

void keyPressed(unsigned char key, int, int) {

    std::cout << "keyPressed(): " << key << std::endl;

    Start ^= 1;
    glutTimerFunc(500, timerEvent, 0);
}

void mouseEvent(int button, int state,
                int x, int y) {

    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        auto nx = x / (PxLineX);
        auto ny = y / (PxLineY);
        std::cout << "Mouse event ( " << nx << " " << ny << ")" << std::endl;
        if (nx < inMap.max_x && ny < inMap.max_y) inMap.cell[nx][inMap.max_y - 1 - ny].pow ^= 1;
        display();
    }
}

void displayNet() {
    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(0, 0);
    glVertex2i(ResolutionX, 0);

    glVertex2i(ResolutionX, 0);
    glVertex2i(ResolutionX, ResolutionY);

    glVertex2i(ResolutionX, ResolutionY);
    glVertex2i(0, ResolutionY);

    glVertex2i(0, ResolutionY);
    glVertex2i(0, 0);

    glColor3f(0.2, 0.2, 0.2);

    for (int i = 0; i < ResolutionX; i += PxLineX) {

        glVertex2i(i, 0);
        glVertex2i(i, ResolutionY);
    }

    for (int i = 0; i < ResolutionY; i += PxLineY) {
        glVertex2i(0, i);
        glVertex2i(ResolutionX, i);
    }
    glEnd();
}

void displayLifeMap(const LifeMap& map) {
    for (int j = 0; j < map.max_y; j++)
        for (int i = 0; i < map.max_x; i++) {
            if (map.cell[i][j].pow > 0) {
                glBegin(GL_QUADS);
                glColor3f(1.0, 1.0, 1.0);
                glVertex2i(i * PxLineX, j * PxLineY);
                glVertex2i((i + 1) * PxLineX, j * PxLineY);
                glVertex2i((i + 1) * PxLineX, (j + 1) * PxLineY);
                glVertex2i(i * PxLineX, (j + 1) * PxLineY);
                glEnd();
            }
        }
}

void timerEvent(int value) {
    if (Start) {
        lifeQuant(inMap, outMap);
        inMap = outMap;
        display();
        glutTimerFunc(TimeDelay, timerEvent, 1);
    }
}