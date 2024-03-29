#include "cell.h"
#include "coord.h"
#include "field.h"
#include "life.h"
#include <atomic>
#include <iostream>
#include <memory>
#include <options.h>
#include <vector>

#if defined(linux) || defined(_WIN32)
    #include <GL/glut.h> /*Для Linux и Windows*/
#else
    #include <GLUT/GLUT.h> /*Для Mac OS*/
#endif

namespace {
    constexpr auto GameTitle = "OpenGL Life Game";

    double PxLineX = 0;
    double PxLineY = 0;

    std::atomic_int8_t Start{0};
    std::unique_ptr<Life> life;
    Options options;

    Coord windowResolution;
    Coord cellNumber;
}// namespace

void reshape(int w, int h);
void display();
void keyPressed(unsigned char key, int x, int y);
void mouseEvent(int button, int state, int x, int y);
void displayLifeMap(const Life& map);
void displayNet(const Coord& windowResolution);
void timerEvent(int value);

int main(int argc, char* argv[]) {
    try {
        if (!options.LoadFromCmdLine(argc, argv)) {
            std::cerr << "Invalid command line arguments";
            return EXIT_FAILURE;
        }
        windowResolution = options.GetWindowResolution();
        cellNumber = options.GetCellNumber();
        PxLineX = static_cast<float>(windowResolution.X) / cellNumber.X;
        PxLineY = static_cast<float>(windowResolution.Y) / cellNumber.Y;

        life = std::make_unique<Life>(options.GetCellNumber());

        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*Включаем двойную буферизацию и четырехкомпонентный цвет*/

        glutInitWindowSize(windowResolution.X, windowResolution.Y);
        glutCreateWindow("OpenGL Life Game");

        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutKeyboardFunc(keyPressed);
        glutMouseFunc(mouseEvent);

        timerEvent(0);

        glutMainLoop();
    } catch (std::exception& e) {
        std::cerr << "ERR: " << e.what();
    }

    return EXIT_SUCCESS;
}


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    windowResolution.X = w;
    windowResolution.Y = h;
    PxLineX = static_cast<float>(windowResolution.X) / cellNumber.X;
    PxLineY = static_cast<float>(windowResolution.Y) / cellNumber.Y;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    displayNet(windowResolution);
    displayLifeMap(*life);
    glutSwapBuffers();
}

void keyPressed(unsigned char key, int, int) {
    std::cout << "keyPressed(): " << key << std::endl;

    Start.fetch_xor(1);
    glutSetWindowTitle((std::string(GameTitle) + (Start.load() ? " Start" : " Stop")).c_str());
}

void mouseEvent(int button, int state, int x, int y) {
    if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
        const int32_t nx = x / (PxLineX);
        const int32_t ny = y / (PxLineY);
        std::cout << "Mouse event ( " << nx << " " << ny << ")" << std::endl;
        const auto dimension = life->GetDimension();
        if (nx < dimension.X && ny < dimension.Y) {
            life->GetCell({nx, dimension.Y - ny - 1}).pow ^= 1;
        }
        display();
    }
}

void displayNet(const Coord& wRes) {

    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2i(0, 0);
    glVertex2i(wRes.X, 0);

    glVertex2i(wRes.X, 0);
    glVertex2i(wRes.X, wRes.Y);

    glVertex2i(wRes.X, wRes.Y);
    glVertex2i(0, wRes.Y);

    glVertex2i(0, wRes.Y);
    glVertex2i(0, 0);

    glColor3f(0.2, 0.2, 0.2);

    for (float i = 0; i < wRes.X; i += PxLineX) {
        glVertex2f(i, 0);
        glVertex2f(i, wRes.Y);
    }

    for (float i = 0; i < wRes.Y; i += PxLineY) {
        glVertex2f(0, i);
        glVertex2f(wRes.X, i);
    }
    glEnd();
}

void displayLifeMap(const Life& map) {
    map.ForEachCell([](const Coord& c, const Cell& item) {
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
    if (Start.load()) {
        life->Quant();
        display();
    }
    glutTimerFunc(std::chrono::duration_cast<std::chrono::milliseconds>(options.GetQuantTime()).count(), timerEvent, 1);
}