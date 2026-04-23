#include <GL/glut.h>

// ekhane global variable
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

