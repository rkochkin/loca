#include "cell.h"
#include "coord.h"
#include "field.h"
#include "life.h"
#include <iostream>
#include <memory>
#include <vector>

#if defined(linux) || defined(_WIN32)
#include <GL/glut.h> /*Для Linux и Windows*/
#else
#include <GLUT/GLUT.h> /*Для Mac OS*/
#endif

namespace {
    constexpr auto GameTitle = "OpenGL Life Game";
    constexpr auto ResolutionX = 800;
    constexpr auto ResolutionY = 800;
    constexpr uint32_t CellNumberX = 100;
    constexpr uint32_t CellNumberY = 100;
    constexpr auto TimeDelay = 100;

    constexpr auto PxLineX = ResolutionX / CellNumberX;
    constexpr auto PxLineY = ResolutionY / CellNumberY;

    bool Start = false;
}// namespace

Cell lifeAct(const CellMatrix& cellMatrix, const Coord& selfCoord) {
    Cell cell = cellMatrix.Get(selfCoord);
    auto sum = cellMatrix.NeighborsSum(selfCoord);
    if (cellMatrix.Get(selfCoord).pow > 0) {// Если клетка живая
        if (sum == 2 || sum == 3)           // Если есть 2 или 3 живые соседки
            cell.pow = 1;                   // то клетка продолжает жить
        else
            cell.pow = 0;// иначе умирает
    } else {             // Если пусто
        if (sum == 3)    // Если есть ровно 3 живые соседки
            cell.pow = 1;// зарождается жизнь в клетке
        else
            cell.pow = 0;
    }
    return cell;
}


void reshape(int w, int h);
void display();
void keyPressed(unsigned char key, int x, int y);
void mouseEvent(int button, int state, int x, int y);
void displayLifeMap(const Life& map);
void displayNet();
void timerEvent(int value);

std::unique_ptr<Life> life;

int main(int argc, char* argv[]) {
    try {
        life = std::make_unique<Life>(Coord{CellNumberX, CellNumberY}, lifeAct, 0);

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*Включаем двойную буферизацию и четырехкомпонентный цвет*/
        glutInitWindowSize(ResolutionX, ResolutionY);
        glutCreateWindow("OpenGL Life Game");

        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutKeyboardFunc(keyPressed);
        glutMouseFunc(mouseEvent);


        glutMainLoop();
    } catch (std::exception& e) {
        std::cerr << "ERR: " << e.what();
    }

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
    displayLifeMap(*life);
    glutSwapBuffers();
}

void keyPressed(unsigned char key, int, int) {
    std::cout << "keyPressed(): " << key << std::endl;

    Start ^= 1;
    glutTimerFunc(500, timerEvent, 0);
    glutSetWindowTitle((std::string(GameTitle) + (Start ? " Start" : " Stop")).c_str());
}

void mouseEvent(int button, int state,
                int x, int y) {
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        uint32_t nx = x / (PxLineX);
        uint32_t ny = y / (PxLineY);
        std::cout << "Mouse event ( " << nx << " " << ny << ")" << std::endl;
        if (nx < life->m_dimension.X && ny < life->m_dimension.Y) {
            life->cellMatrix.Get({nx, life->m_dimension.Y - 1 - ny}).pow ^= 1;
        }
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

void displayLifeMap(const Life& map) {
    life->cellMatrix.ForEach([](const Coord& c, const Cell& item) {
        if (item.pow > 0) {
            glBegin(GL_QUADS);
            glColor3f(1.0, 1.0, 1.0);
            glVertex2i(c.X * PxLineX, c.Y * PxLineY);
            glVertex2i((c.X + 1) * PxLineX, c.Y * PxLineY);
            glVertex2i((c.X + 1) * PxLineX, (c.Y + 1) * PxLineY);
            glVertex2i(c.X * PxLineX, (c.Y + 1) * PxLineY);
            glEnd();
        }
    });
}

void timerEvent(int) {
    if (Start) {
        life->Quant();
        display();
        glutTimerFunc(TimeDelay, timerEvent, 1);
    }
}