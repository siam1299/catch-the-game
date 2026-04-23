#include <GL/glut.h>
#include <stdio.h>

// Global variables
float basketX = 300.0f;
float basketY = 40.0f;
int score = 0;

// Function prototypes
void drawBasket();
void moveBasketKeyboard(int key, int x, int y);
void moveBasketMouse(int x, int y);
void checkCatch(float objectX, float objectY, int points);
void updateScore(int points);
void drawScoreText();
void display();

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
    basketX = (float)x;

    if (basketX < 30) basketX = 30;
    if (basketX > 770) basketX = 770;

    glutPostRedisplay();
}

void checkCatch(float objectX, float objectY, int points) {
    if (objectY <= basketY + 20 &&
        objectX >= basketX - 30 &&
        objectX <= basketX + 30) {
        updateScore(points);
    }
}

void updateScore(int points) {
    score += points;
}

void drawScoreText() {
    char text[50];
    sprintf(text, "Score: %d", score);

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(20.0f, 570.0f);

    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawBasket();
    drawScoreText();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Catch The Eggs");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glutDisplayFunc(display);
    glutSpecialFunc(moveBasketKeyboard);
    glutPassiveMotionFunc(moveBasketMouse);

    glutMainLoop();
    return 0;
}