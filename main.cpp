#include <GL/glut.h>

// Global variables
float basketX = 300.0f;
float basketY = 40.0f;
int score = 0;

void drawBasket() {
    glColor3f(1.0f, 1.0f, 0.0f);

    glBegin(GL_POLYGON);
        glVertex2f(basketX - 30, basketY);
        glVertex2f(basketX + 30, basketY);
        glVertex2f(basketX + 20, basketY + 20);
        glVertex2f(basketX - 20, basketY + 20);
    glEnd();
}

void moveBasketKeyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        basketX -= 20.0f;
    }
    else if (key == GLUT_KEY_RIGHT) {
        basketX += 20.0f;
    }

    if (basketX < 30) basketX = 30;
    if (basketX > 770) basketX = 770;

    glutPostRedisplay();
}

void moveBasketMouse(int x, int y) {

}

void checkCatch() {

}

void updateScore(int points) {

}

void drawScoreText() {

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBasket();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Catch The Eggs");

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glutDisplayFunc(display);
    glutSpecialFunc(moveBasketKeyboard);

    glutMainLoop();
    return 0;
}